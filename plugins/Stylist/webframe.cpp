#include "webframe.h"

#include "plugin.h"
#include "styles.h"
#include "utility.h"

#include <cassert>
#include <fstream>
#include <sstream>
#include <type_traits>

#include <QtCore/QDebug>
#include <QtWebKit/QWebElement>
#include <QtWebKit/QWebFrame>

namespace stylist {

WebFrame::
WebFrame(QWebFrame* const frame)
    : m_frame { frame }
{
    qDebug() << __FUNCTION__ << this << frame;

    assert(frame);

    this->connect(frame, SIGNAL(initialLayoutCompleted()),
                  this,  SLOT(insertStyle()));
    this->connect(&Plugin::styles(), SIGNAL(changed()),
                  this,              SLOT(insertStyle()));

    assert(m_frame);
}

static char styleId[] = "__stylist__";

void WebFrame::
removeStyle()
{
    static const std::string &selector =
        str(boost::format { "head > style[id=\"%s\"]" } % styleId);

    auto &&style =
        m_frame->findFirstElement(selector.c_str());
    if (!style.isNull()) {
        qDebug() << "removing style";
        style.removeFromDocument();
    }
}

void WebFrame::
insertStyle()
{
    qDebug() << __FUNCTION__ << this;

    const auto &url = m_frame->url();
    if (!url.isValid()) return;

    qDebug() << url;
    removeStyle();

    const auto &style = Plugin::styles().query(url);
    if (style.empty()) return;
    qDebug() << "matched" << url;

    std::ostringstream oss;
    oss << "<style id=\"" << styleId << "\" type=\"text/css\">"
        << style
        << "</style>";
    const auto &html = oss.str();

    auto &&head = m_frame->findFirstElement("head");
    assert(!head.isNull());

    head.appendInside(html.c_str());
}

} // namespace stylist