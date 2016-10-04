#include "page_adaptor.hpp"

#include "adblock_dash.hpp"
#include "frame_adaptor.hpp"

#include <cassert>

#include <QtCore/QString>
#include <QtGui/QPalette>
#include <QtWebKitWidgets/QWebFrame>
#include <QtWebKitWidgets/QWebPage>

#include <adblock/api.h>

namespace adblock_dash {

RequestContext::
RequestContext(::adblock_context const& cxt)
    : origin { QString::fromUtf8(cxt.origin, cxt.origin_len) }
    , siteKey { QString::fromUtf8(cxt.site_key, cxt.site_key_len) }
    , contentType { static_cast<ContentType>(cxt.content_type) }
    , isPopup { cxt.is_popup }
{}

PageAdaptor::
PageAdaptor(QWebPage& page, AdBlockDash& adBlockDash)
    : m_page { page }
    , m_adBlockDash { adBlockDash }
{
    this->connect(
        &page, SIGNAL(frameCreated(QWebFrame*)),
         this,   SLOT(onFrameCreated(QWebFrame*))
    );

    auto* const mainFrame = page.mainFrame();
    assert(mainFrame);
    onFrameCreated(mainFrame);

    this->connect(
        mainFrame, SIGNAL(urlChanged(const QUrl&)),
        this,        SLOT(onUrlChanged(const QUrl&))
    );
}

static QString
toString(RequestContext::ContentType const contentType)
{
    switch (contentType) {
    case RequestContext::ContentType::TYPE_OTHER:
        return "other";
    case RequestContext::ContentType::TYPE_SCRIPT:
        return "script";
    case RequestContext::ContentType::TYPE_IMAGE:
        return "image";
    case RequestContext::ContentType::TYPE_STYLESHEET:
        return "style sheet";
    case RequestContext::ContentType::TYPE_OBJECT:
        return "object";
    case RequestContext::ContentType::TYPE_DOCUMENT:
        return "document";
    case RequestContext::ContentType::TYPE_SUBDOCUMENT:
        return "subdocument";
    case RequestContext::ContentType::TYPE_XMLHTTPREQUEST:
        return "XMLHTTPRequest";
    case RequestContext::ContentType::TYPE_OBJECT_SUBREQUEST:
        return "object subrequest";
    case RequestContext::ContentType::TYPE_MEDIA:
        return "font";
    case RequestContext::ContentType::TYPE_FONT:
        return "media";
    case RequestContext::ContentType::TYPE_WEBSOCKET:
        return "websocket";
    default:
        assert(false && "unknown content type");
    }
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
    new FrameAdaptor { *frame, m_adBlockDash }; // frame will take ownership
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
