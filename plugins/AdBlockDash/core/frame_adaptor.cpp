#include "frame_adaptor.hpp"

#include "adblock_dash.hpp"
#include "common/logging.hpp"

#include <QtCore/QString>
#include <QtWebKit/QWebElement>
#include <QtWebKitWidgets/QWebFrame>

namespace adblock_dash {

FrameAdaptor::
FrameAdaptor(QWebFrame &frame, AdBlockDash &adBlockDash_)
    : m_frame { frame }
    , m_adBlockDash { adBlockDash_ }
{
    qCDebug(adBlockDash) << __func__ << this;

    if (m_adBlockDash.enabled()) {
        install();
    }

    this->connect(
        &m_adBlockDash, &AdBlockDash::statusChanged,
        this,           &FrameAdaptor::onStatusChanged);
}

FrameAdaptor::
~FrameAdaptor()
{
    qCDebug(adBlockDash) << __func__ << this;
    //uninstall(); // this will crash because frame is already dying
}

void FrameAdaptor::
install()
{
    qCDebug(adBlockDash) << __func__ << this;

    installCustomObject();

    this->connect(&m_frame, SIGNAL(javaScriptWindowObjectCleared()),
                  this,       SLOT(installCustomObject()));
    this->connect(&m_frame, SIGNAL(initialLayoutCompleted()),
                  this,       SLOT(injectElementHidingCss()));
}

void FrameAdaptor::
uninstall()
{
    qCDebug(adBlockDash) << __func__ << this;

    uninstallCustomObject();
    removeElementHidingCss();

    this->disconnect(&m_frame, SIGNAL(javaScriptWindowObjectCleared()),
                     this,       SLOT(installCustomObject()));
    this->disconnect(&m_frame, SIGNAL(initialLayoutCompleted()),
                     this,       SLOT(injectElementHidingCss()));
}

bool FrameAdaptor::
shouldLoad(QString const& url, QWebElement const& element_)
{
    //qCDebug(adBlockDash) << __func__ << this;

    // work around that argument from WebKit binding have to be immutable.
    auto& element = const_cast<QWebElement&>(element_);

    const auto &shouldBlock = m_adBlockDash.shouldBlock(url, element);
    if (shouldBlock) {
        hideElement(element);
    }

    return !shouldBlock;
}

bool FrameAdaptor::
shouldLoadWebSocket(const QString &url) const
{
    qCDebug(adBlockDash) << __func__ << this;

    return !m_adBlockDash.shouldBlockWebSocket(url, m_frame);
}

void FrameAdaptor::
installCustomObject()
{
    qCDebug(adBlockDash) << __func__ << this;

    static const QString script {
        R"(
        function adBlockDashOnBeforeLoad(event) {
            if (!window.adBlockDash.shouldLoad(event.url, event.target)) {
                event.preventDefault();
            }
        };
        document.addEventListener("beforeload",
            adBlockDashOnBeforeLoad, true);
        )"
    };
    m_frame.addToJavaScriptWindowObject("adBlockDash", this);
    m_frame.evaluateJavaScript(script);

    installWebSocketWrapper();
}

void FrameAdaptor::
uninstallCustomObject()
{
    qCDebug(adBlockDash) << __func__ << this;
    static const QString script {
        R"(
        if (adBlockDashOnBeforeLoad) {
            document.removeEventListener(
                "beforeload", adBlockDashOnBeforeLoad, true);
        }
        if (adBlockDash) {
            adBlockDash = null;
        }
        )"
    };
    m_frame.evaluateJavaScript(script);

    uninstallWebSocketWrapper();
}

void FrameAdaptor::
installWebSocketWrapper()
{
    static const QString script {
        R"(
        var adBlockDashOriginalWebSocket = WebSocket;
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
uninstallWebSocketWrapper()
{
    static const QString script {
        R"(
        if (adBlockDashOriginalWebSocket) {
            WebSocket = adBlockDashOriginalWebSocket;
        }
        )"
    };
    m_frame.evaluateJavaScript(script);
}

void FrameAdaptor::
injectElementHidingCss()
{
    qCDebug(adBlockDash) << __func__ << this;

    const auto &url = m_frame.url();
    if (url.isEmpty()) return;

    const auto &css = m_adBlockDash.elementHideCss(url);

    auto &&body = m_frame.findFirstElement("body");
    if (body.isNull()) return;

    body.appendInside(
        "<style id=\"adBlockDashElementHideCss\" type=\"text/css\">"
        + css
        + "</style>");
}

void FrameAdaptor::
removeElementHidingCss()
{
    qCDebug(adBlockDash) << __func__ << this;

    auto&& element = m_frame.findFirstElement("#adBlockDashElementHideCss");
    if (!element.isNull()) {
        qCDebug(adBlockDash) << "removing element hiding CSS";
        element.removeFromDocument();
    }
}

void FrameAdaptor::
onStatusChanged(bool const enabled)
{
    qCDebug(adBlockDash) << __func__ << this;

    if (enabled) {
        install();
    }
    else {
        uninstall();
    }
}

void FrameAdaptor::
hideElement(QWebElement& element)
{
    qCDebug(adBlockDash) << __func__ << this;

    static QString const name = "display";
    static QString const value = "none !important";

    element.setStyleProperty(name, value);
}

} // namespace adblock_dash
