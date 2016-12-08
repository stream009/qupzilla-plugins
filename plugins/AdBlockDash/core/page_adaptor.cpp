#include "page_adaptor.hpp"

#include "adblock_dash.hpp"
#include "frame_adaptor.hpp"
#include "common/logging.hpp"

#include <cassert>

#include <boost/make_unique.hpp>
#include <boost/numeric/conversion/cast.hpp>

#include <QtCore/QString>
#include <QtGui/QPalette>
#include <QtWebKitWidgets/QWebFrame>
#include <QtWebKitWidgets/QWebPage>

#include <adblock/api.h>

namespace adblock_dash {

RequestContext::
RequestContext(::adblock_context const& cxt)
    : origin { QString::fromUtf8(cxt.origin, boost::numeric_cast<int>(cxt.origin_len)) }
    , siteKey { QString::fromUtf8(cxt.site_key, boost::numeric_cast<int>(cxt.site_key_len)) }
    , contentType { static_cast<ContentType>(cxt.content_type) }
    , isPopup { cxt.is_popup }
{}

PageAdaptor::
PageAdaptor(QWebPage& page, AdBlockDash& adBlockDash_)
    : m_page { page }
    , m_adBlockDash { adBlockDash_ }
{
    qCDebug(adBlockDash) << __func__ << this;

    this->connect(
        &m_page, SIGNAL(frameCreated(QWebFrame*)),
         this,     SLOT(onFrameCreated(QWebFrame*))
    );

    auto* const mainFrame = m_page.mainFrame();
    assert(mainFrame);
    onFrameCreated(mainFrame);

    this->connect(
        mainFrame, SIGNAL(urlChanged(const QUrl&)),
        this,        SLOT(onUrlChanged(const QUrl&))
    );
}

//PageAdaptor::~PageAdaptor() = default;
PageAdaptor::
~PageAdaptor()
{
    qCDebug(adBlockDash) << __func__ << this;
}

void PageAdaptor::
dump() const
{
    auto* const frame = m_page.mainFrame();
    assert(frame);

    QString const js { "console.log('%1');" };
    QString const urlFmt { "URL: %1\\n" };
    QString const timeFmt { "Time: %1\\n" };
    QString const contentTypeFmt { "Content Type: %1\\n" };
    QString const originFmt { "Origin: %1\\n" };
    QString const reasonFmt { "Reason: %1\\n" };
    QString const subscriptionFmt { "Subscription: %1\\n\\n" };

    QString msg;
    for (auto const& req: m_blockedRequests) {
        msg =  urlFmt.arg(req.url.toString());
        msg += timeFmt.arg(req.time.toString());
        msg += contentTypeFmt.arg(toString(req.context.contentType));
        msg += originFmt.arg(req.context.origin.toString());
        msg += reasonFmt.arg(req.rule);
        msg += subscriptionFmt.arg(req.subscription);

        frame->evaluateJavaScript(js.arg(msg));
    }
}

void PageAdaptor::
onFrameCreated(QWebFrame* const frame)
{
    assert(frame);

    m_frameAdaptors.emplace(
        frame,
        boost::make_unique<FrameAdaptor>(*frame, m_adBlockDash)
    );

    this->connect(
        frame, &QWebFrame::destroyed,
        this,  &PageAdaptor::onFrameDestroyed);
}

void PageAdaptor::
onFrameDestroyed()
{
    qCDebug(adBlockDash) << __func__ << this;
    // dynamic_cast will fail because QWebFrame part of object will have
    // deleted at this point.
    auto* const frame = reinterpret_cast<QWebFrame*>(this->sender());
    assert(frame);

    auto const count = m_frameAdaptors.erase(frame);
    assert(count != 0); (void)count;
}

void PageAdaptor::
onUrlChanged(QUrl const& url)
{
    auto* const frame = dynamic_cast<QWebFrame*>(this->sender());
    assert(frame);

    auto* const page = frame->page();
    assert(page);

    const auto &urlStr = url.toString();
    if (urlStr == "about:blank" ||
        urlStr.endsWith(".gif") ||
        urlStr.endsWith(".png") ||
        urlStr.endsWith(".jpg") || urlStr.endsWith(".jpeg"))
    {
        page->setPalette(QApplication::palette());
    }
    else {
        QPalette pal = page->palette();
        pal.setBrush(QPalette::Base, Qt::white);
        page->setPalette(pal);
    }
}

} // namespace adblock_dash
