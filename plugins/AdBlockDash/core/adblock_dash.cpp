#include "adblock_dash.hpp"

#include "page_adaptor.hpp"
#include "common/utility.hpp"
#include "common/logging.hpp"
#include "settings.hpp"

#include <cassert>

#include <boost/optional.hpp>
#include <boost/range/algorithm.hpp>

#include <QtCore/QIODevice>
#include <QtWidgets/QMenu>
#include <QtWidgets/QAction>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QtNetwork/QNetworkRequest>
#include <QtWebKit/QWebElement>
#include <QtWebKitWidgets/QWebFrame>
#include <QtWidgets/QApplication>

#include <webpage.h>

namespace adblock_dash {

static bool
isScript(const QNetworkRequest &request)
{
    const QString &url = request.url().toString();
    return url.endsWith(QL1S(".js"));
}

static bool
isImage(const QNetworkRequest &request)
{
    const QString &url = request.url().toString();
    return url.endsWith(QL1S(".png")) ||
           url.endsWith(QL1S(".jpg")) ||
           url.endsWith(QL1S(".gif")) ||
           url.endsWith(QL1S(".jpeg"));
}

static bool
isStyleSheet(const QNetworkRequest &request)
{
    const QString &url = request.url().toString();
    return url.endsWith(QL1S(".css"));
}

static bool
isObject(const QNetworkRequest &request)
{
    const auto &key =
        static_cast<QNetworkRequest::Attribute>(QNetworkRequest::User + 150);

    const auto &attribute = request.attribute(key).toString();

    return attribute == "object";
}

static bool
isXmlHttpRequest(const QNetworkRequest &request)
{
    return request.rawHeader("X-Requested-With")
                                        == QByteArray("XMLHttpRequest");
}

static bool
isDocument(const QNetworkRequest &request)
{
    const QString &url = request.url().toString();
    if (!url.endsWith(QL1S(".html")) && !url.endsWith(QL1S(".htm"))) {
        return false;
    }

    QWebFrame* originatingFrame =
                 dynamic_cast<QWebFrame*>(request.originatingObject());
    if (!originatingFrame) {
        return false;
    }

    QWebPage* page = originatingFrame->page();
    if (!page) {
        return false;
    }

    return originatingFrame == page->mainFrame();
}

static bool
isSubDocument(const QNetworkRequest &request)
{
    const QString &url = request.url().toString();
    if (!url.endsWith(QL1S(".html")) && !url.endsWith(QL1S(".htm"))) {
        return false;
    }

    QWebFrame* originatingFrame =
                    dynamic_cast<QWebFrame*>(request.originatingObject());
    if (!originatingFrame) {
        return false;
    }

    QWebPage* page = originatingFrame->page();
    if (!page) {
        return false;
    }

    return originatingFrame != page->mainFrame();
}

static bool
isFont(const QNetworkRequest &request)
{
    const QString &url = request.url().toString();
    return url.endsWith(QL1S(".ttf")) ||
           url.endsWith(QL1S(".otf")) ||
           url.endsWith(QL1S(".woff")) ||
           //url.endsWith(QL1S(".svg")) || //TODO SVG could be image
           url.endsWith(QL1S(".eot"));
}

static bool
isObjectSubRequest(const QNetworkRequest &request)
{
    QWebFrame* originatingFrame =
                    dynamic_cast<QWebFrame*>(request.originatingObject());
    return originatingFrame == NULL;
}

static bool
isMedia(const QNetworkRequest&)
{
    return false;
}

static enum content_type
guessContentType(const QNetworkRequest &request)
{
    if (isScript(request)) {
        return TYPE_SCRIPT;
    }
    else if (isImage(request)) {
        return TYPE_IMAGE;
    }
    else if (isStyleSheet(request)) {
        return TYPE_STYLESHEET;
    }
    else if (isObject(request)) {
        return TYPE_OBJECT;
    }
    else if (isDocument(request)) {
        return TYPE_DOCUMENT;
    }
    else if (isSubDocument(request)) {
        return TYPE_SUBDOCUMENT;
    }
    else if (isXmlHttpRequest(request)) {
        return TYPE_XMLHTTPREQUEST;
    }
    else if (isObjectSubRequest(request)) {
        return TYPE_OBJECT_SUBREQUEST;
    }
    else if (isFont(request)) {
        return TYPE_FONT;
    }
    else if (isMedia(request)) {
        return TYPE_MEDIA;
    }
    else {
        return TYPE_OTHER;
    }
}

static boost::optional<enum ::content_type>
guessContentType(const QWebElement &element)
{
    const auto &tag = element.tagName().toUpper();
    if (tag == "IMG" || tag == "INPUT") {
        return TYPE_IMAGE;
    }
    else if (tag == "SCRIPT") {
        return TYPE_SCRIPT;
    }
    else if (tag == "EMBED" || tag == "OBJECT") {
        return TYPE_OBJECT;
    }
    else if (tag == "IFRAME") {
        return TYPE_SUBDOCUMENT;
    }
    else if (tag == "AUDIO" || tag == "VIDEO") {
        return TYPE_MEDIA;
    }
    else if (tag == "LINK") {
        const auto &rel = element.attribute("REL").toUpper();
        const auto &type = element.attribute("TYPE").toUpper();
        if (rel == "STYLESHEET" || rel == "ALTERNATE STYLESHEET") {
            return TYPE_STYLESHEET;
        }
        else if (rel.endsWith("ICON") || type.startsWith("IMAGE")) {
            return TYPE_IMAGE;
        }
        else {
            qCWarning(adBlockDash)
                << "Unimplemented link relationship:" << rel << type;
            return boost::none;
        }
    }
    else {
        qCWarning(adBlockDash) << "Unimplemented tag:" << tag;
        return boost::none;
    }
}

static QUrl
originatingUrl(const QWebFrame &frame)
{
    const auto &url = frame.url();
    if (url.isEmpty()) {
        auto* const page = frame.page();
        assert(page);
        auto* const mainFrame = page->mainFrame();
        assert(mainFrame);
        return mainFrame->url();
    }
    else {
        return url;
    }
}

static QUrl
normalizeUrl(const QUrl &base, const QUrl &url)
{
    if (base.isEmpty() && url.isRelative()) {
        qCWarning(adBlockDash)
            << "Empty base on relative URL:" << url.toEncoded();
    }
    return url.isRelative() ? base.resolved(url) : url;
}


AdBlockDash::
AdBlockDash(Settings &settings, QNetworkAccessManager &manager)
    : m_adBlock { ::adblock_create() }
    , m_settings { settings }
    , m_networkManager { manager }
    , m_downloader { settings, manager }
    , m_updater { settings, manager }
{
    assert(m_adBlock);

    addFilterSets();

    auto &notifier = m_settings.subscriptions().notifier();
    this->connect(
        &notifier, SIGNAL(appended(const Subscription&)),
        this,        SLOT(onSubscriptionAppended(const Subscription&)));
    this->connect(
        &m_downloader, SIGNAL(finished(
                              const QString&, const QUrl&, const QString&)),
        this,            SLOT(onDownloadCompleted(
                              const QString&, const QUrl&, const QString&)));
    this->connect(
        &m_downloader, SIGNAL(error(
                              const QString&, const QUrl&, const QString&)),
        this,            SLOT(onDownloadError(
                              const QString&, const QUrl&, const QString&)));

    this->connect(&m_updater, SIGNAL(finished()),
                  this,         SLOT(reload()));
    m_updater();
}

AdBlockDash::
~AdBlockDash()
{
    setEnabled(false);
    ::adblock_destroy(m_adBlock);
}

// This overload will be called by request originated from web page element.
bool AdBlockDash::
shouldBlock(const QString &url, const QWebElement &element) const
{
    if (!m_enabled) return false;

    ::adblock_context context;
    ::memset(&context, 0, sizeof(context));

    const auto &contentType = guessContentType(element);
    if (!contentType) {
        return false;
    }
    context.content_type = *contentType;

    auto* const frame = element.webFrame();
    assert(frame);
    const auto &origin = originatingUrl(*frame);

    const auto &originUtf8 = origin.toEncoded();
    if (!originUtf8.isEmpty()) {
        context.origin = originUtf8.constData();
        context.origin_len = ::toSizeT(originUtf8.size());
    }

    const char *subscriptionPtr = nullptr;
    size_t subscriptionLen = 0;
    const char *reasonPtr = nullptr;
    size_t reasonLen;

    const QUrl &theUrl = normalizeUrl(origin, { url });
    const auto &buf = theUrl.toEncoded();

    auto const result = ::adblock_should_block(
        m_adBlock,
        buf.constData(), ::toSizeT(buf.size()), &context,
        &subscriptionPtr, &subscriptionLen,
        &reasonPtr, &reasonLen
    );

    if (result) {
        logBlockedRequest(*frame, theUrl,
                QString::fromUtf8(reasonPtr, reasonLen),
                QString::fromUtf8(subscriptionPtr, subscriptionLen),
                context
        );
    }

    return result;
}

// This overload is called from request made by script, object, etc..
bool AdBlockDash::
shouldBlock(const QNetworkRequest &request) const
{
    if (!m_enabled) return false;

    auto* const frame =
                    dynamic_cast<QWebFrame*>(request.originatingObject());
    if (!frame) return false;

    const auto &url = request.url();
    assert(!url.isEmpty());

    if (!url.isValid()) return false;

    ::adblock_context context;
    memset(&context, 0, sizeof(context));

    context.content_type = guessContentType(request);

    const auto &origin = originatingUrl(*frame);
    const auto &originUtf8 = origin.toEncoded();
    if (!originUtf8.isEmpty()) {
        context.origin = originUtf8.data();
        context.origin_len = ::toSizeT(originUtf8.size());
    }

    const char *subscriptionPtr = nullptr;
    size_t subscriptionLen = 0;
    const char *reasonPtr = nullptr;
    size_t reasonLen;

    const auto &utf8 = url.toEncoded();
    assert(!utf8.isEmpty());

    auto const result = ::adblock_should_block(
                m_adBlock,
                utf8.constData(), ::toSizeT(utf8.size()), &context,
                &subscriptionPtr, &subscriptionLen,
                &reasonPtr, &reasonLen
           );

    if (result) {
        logBlockedRequest(*frame, url,
                QString::fromUtf8(reasonPtr, reasonLen),
                QString::fromUtf8(subscriptionPtr, subscriptionLen),
                context
        );
    }

    return result;
}

bool AdBlockDash::
shouldBlockWebSocket(const QString &url, const QWebFrame &frame) const
{
    return shouldBlock(url, frame, TYPE_WEBSOCKET);
}

bool AdBlockDash::
shouldBlock(const QString &url, const QWebFrame &frame,
                             const enum ::content_type type) const
{
    if (!m_enabled) return false;

    ::adblock_context context;
    memset(&context, 0, sizeof(context));

    context.content_type = type;

    const auto &origin = originatingUrl(frame);
    const auto &originUtf8 = origin.toEncoded();
    if (!originUtf8.isEmpty()) {
        context.origin = originUtf8.data();
        context.origin_len = ::toSizeT(originUtf8.size());
    }

    const char *subscriptionPtr = nullptr;
    size_t subscriptionLen = 0;
    const char *reasonPtr = nullptr;
    size_t reasonLen;

    const auto &utf8 = url.toUtf8();
    assert(!utf8.isEmpty());

    auto const result = ::adblock_should_block(
                m_adBlock,
                utf8.constData(), ::toSizeT(utf8.size()), &context,
                &subscriptionPtr, &subscriptionLen,
                &reasonPtr, &reasonLen
           );

    if (result) {
        logBlockedRequest(frame, url,
                QString::fromUtf8(reasonPtr, reasonLen),
                QString::fromUtf8(subscriptionPtr, subscriptionLen),
                context
        );
    }

    return result;
}

QString AdBlockDash::
elementHideCss(const QUrl &url) const
{
    const auto &utf8 = url.toEncoded();

    const char *css = nullptr;
    size_t cssLen = 0;
    ::adblock_element_hide_css(
        m_adBlock,
        utf8.constData(), ::toSizeT(utf8.size()),
        &css, &cssLen
    );

    if (cssLen == 0) return {};

    const auto &result = QString::fromUtf8(css, ::toSignedInt(cssLen));

    const auto &freed = ::adblock_free(css);
    if (!freed) {
        qCWarning(adBlockDash) << __func__
                 << QString("fail to free memory: addr = %1, len = %2")
                        .number(reinterpret_cast<unsigned int const>(css), 16)
                        .arg(cssLen);
    }

    return result;
}

QMenu& AdBlockDash::
pageMenu(QWebPage& page) const
{
    auto* const menu = new QMenu { "ADBlock Dash" }; // QtWebKit will take ownership
    assert(menu);

    auto const it = m_pageAdaptors.find(&page);
    assert(it != m_pageAdaptors.end());

    auto const& adaptor = it->second;
    assert(adaptor);

    {
        auto* const action = new QAction { "Dump blocked request", menu };
        assert(action);
        assert(action->parent() == menu);

        this->connect(action,        &QAction::triggered,
                      adaptor.get(), &PageAdaptor::dump);

        menu->addAction(action);
    }
    {
        auto* const action = new QAction { "Disable", menu };
        assert(action);
        assert(action->parent() == menu);
        action->setCheckable(true);
        action->setChecked(!m_enabled);

        this->connect(action, &QAction::triggered,
                      this,   &AdBlockDash::onDisableTriggered);

        menu->addAction(action);
    }

    return *menu;
}

void AdBlockDash::
subscribe(const QString &title, const QUrl &location)
{
    assert(!title.isNull());
    assert(location.isValid());

    m_downloader(title, location); //TODO catch
}

void AdBlockDash::
addFilterSet(const Path &path)
{
    ::adblock_add_filter_set(
                m_adBlock, path.c_str(), std::strlen(path.c_str()));
}

void AdBlockDash::
removeFilterSet(const Path &path)
{
    const auto rc = ::adblock_remove_filter_set(
                    m_adBlock, path.c_str(), std::strlen(path.c_str()));
    assert(rc);
}

void AdBlockDash::
reload()
{
    qCInfo(adBlockDash) << "Reloading ADBlock Dash";
    const auto rc = ::adblock_destroy(m_adBlock);
    assert(rc); (void)rc;

    m_adBlock = ::adblock_create();
    addFilterSets();
}

void AdBlockDash::
addFilterSets()
{
    auto &&subscriptions = m_settings.subscriptions();
    for (const auto &subscription: subscriptions) {
        const auto &path = subscription.path();
        ::adblock_add_filter_set(
                m_adBlock, path.c_str(), std::strlen(path.c_str()));
    }

    auto &&customFilterSets = m_settings.customFilterSets();
    for (const auto &filterSets: customFilterSets) {
        const auto &path = filterSets.path();
        ::adblock_add_filter_set(
                m_adBlock, path.c_str(), std::strlen(path.c_str()));
    }

    const auto &exceptionRules = m_settings.exceptionRules();
    if (exceptionRules.enabled()) {
        const auto &path = exceptionRules.path();

        if (boost::filesystem::exists(path)) {
            ::adblock_add_filter_set(
                    m_adBlock, path.c_str(), std::strlen(path.c_str()));
        }
    }
}

void AdBlockDash::
onDownloadCompleted(const QString &title, const QUrl &location,
                    const QString &path)
{
    auto &&subscriptions = m_settings.subscriptions();

    const Path path_ { path.toLocal8Bit().constData() };
    subscriptions.emplace_back(title, path_, location, true, true);

    Q_EMIT subscribed(title, location);
}

void AdBlockDash::
onDownloadError(const QString &title, const QUrl &url,
                const QString &message)
{
    Q_EMIT subscribeError(title, url, message);
}

void AdBlockDash::
onWebPageCreated(WebPage* const page)
{
    assert(page);

    auto const& result = m_pageAdaptors.emplace(
        page,
        boost::make_unique<PageAdaptor>(*page, *this)
    );
    assert(result.second); (void)result;
}

void AdBlockDash::
onWebPageDeleted(WebPage* const page)
{
    auto const count = m_pageAdaptors.erase(page);
    assert(count != 0); (void)count;
}

void AdBlockDash::
onSubscriptionAppended(const Subscription &subscription)
{
    const auto &path = subscription.path();
    ::adblock_add_filter_set(
                 m_adBlock, path.c_str(), std::strlen(path.c_str()));
}

void AdBlockDash::
onDisableTriggered(bool const checked)
{
    qCDebug(adBlockDash) << __func__ << this << checked;

    setEnabled(!checked);
}

void AdBlockDash::
setEnabled(bool const enabled)
{
    qCDebug(adBlockDash) << __func__ << this << enabled;

    if (m_enabled == enabled) return;

    m_enabled = enabled;
    Q_EMIT statusChanged(m_enabled);
}

static QString
lookupSubscriptionName(Settings& settings, QString const& path)
{
    auto const& subscriptions = settings.subscriptions();

    auto const it = std::find_if(
        subscriptions.begin(), subscriptions.end(),
        [&](Subscription const& entry) {
            return entry.path() == path.toUtf8().data();
        });
    if (it != subscriptions.end()) {
        return it->name();
    }
    else {
        auto const& customFilterSets = settings.customFilterSets();

        auto const it = std::find_if(
            customFilterSets.begin(), customFilterSets.end(),
            [&](FilterSet const& entry) {
                return entry.path() == path.toUtf8().data();
            });
            assert(it != customFilterSets.end());

            return it->name();
    }
}

void AdBlockDash::
logBlockedRequest(
        const QWebFrame& frame,
        QUrl const& url,
        QString const& reason,
        QString const& subscription,
        ::adblock_context const& context) const
{
    auto* const page = frame.page();
    assert(page);

    auto const& it = m_pageAdaptors.find(page);
    if (it == m_pageAdaptors.end()) return; // it happens.

    auto const& adaptor = it->second;
    assert(adaptor);

    BlockedRequest req {
        QDateTime::currentDateTime(),
        url, reason,
        lookupSubscriptionName(m_settings, subscription), context
    };

    qCInfo(adBlockDash) << "Request has Blocked\n" << req;
    adaptor->blockedRequests().push_back(req);
}

} // namespace adblock_dash
