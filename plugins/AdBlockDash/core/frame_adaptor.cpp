#include "frame_adaptor.hpp"

#include "adblock_dash.hpp"

#include <QtCore/QString>
#include <QtWebKit/QWebElement>
#include <QtWebKitWidgets/QWebFrame>

namespace adblock_dash {

FrameAdaptor::
FrameAdaptor(QWebFrame &frame, AdBlockDash &adBlockDash)
    : Base { &frame },
      m_frame { frame },
      m_adBlockDash { adBlockDash }
{
    installCustomObject();
    this->connect(&frame, SIGNAL(javaScriptWindowObjectCleared()),
                  this,     SLOT(installCustomObject()));
    this->connect(&frame, SIGNAL(initialLayoutCompleted()),
                  this,     SLOT(injectElementHidingCss()));
}

bool FrameAdaptor::
shouldLoad(const QString &url, const QWebElement &element) const
{
    const auto &shouldBlock = m_adBlockDash.shouldBlock(url, element);
    if (shouldBlock) {
        const_cast<QWebElement&>(element).setStyleProperty(
                                        "display", "none !important");
    }

    return !shouldBlock;
}

bool FrameAdaptor::
shouldLoadWebSocket(const QString &url) const
{
    return !m_adBlockDash.shouldBlockWebSocket(url, m_frame);
}

void FrameAdaptor::
installCustomObject()
{
    static const QString script {
        R"(
        document.addEventListener("beforeload", function (event) {
            if (!window.adBlockDash.shouldLoad(event.url, event.target)) {
                event.preventDefault();
            }
        }, true);
        )"
    };
    m_frame.addToJavaScriptWindowObject("adBlockDash", this);
    m_frame.evaluateJavaScript(script);

    installWebSocketWrapper();
}

void FrameAdaptor::
installWebSocketWrapper()
{
    static const QString script {
        R"(
        WebSocket = (function (original) {
            function wrapped(url, protocols) {
                if (!window.adBlockDash.shouldLoadWebSocket(url)) {
                    throw "SECURITY_ERR";
                }
                return new original(url, protocols);
            }
            wrapped.prototype = Object.create(original.prototype);

            Object.defineProperties(wrapped, {
                CONNECTING: { value: original.CONNECTING, enumerable: true },
                OPEN: { value: original.OPEN, enumerable: true },
                CLOSING: { value: original.CLOSING, enumerable: true },
                CLOSED: { value: original.CLOSED, enumerable: true },
                prototype: { value: original.prototype }
            });

            return wrapped;
        })(WebSocket);
        )"
    };
    m_frame.evaluateJavaScript(script);
}

void FrameAdaptor::
injectElementHidingCss()
{
    const auto &url = m_frame.url();
    if (url.isEmpty()) return;

    const auto &css = m_adBlockDash.elementHideCss(url);

    auto &&body = m_frame.findFirstElement("body");
    if (body.isNull()) return;

    body.appendInside("<style type=\"text/css\">" + css + "</style>");
}

} // namespace adblock_dash
