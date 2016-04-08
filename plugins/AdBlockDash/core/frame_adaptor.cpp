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
