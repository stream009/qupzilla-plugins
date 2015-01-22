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
                  this,  SLOT(slotInitialLayoutCompleted()));

    assert(m_frame);
}

void WebFrame::
slotInitialLayoutCompleted()
{
    qDebug() << __FUNCTION__ << this;

    const auto &url = m_frame->url();
    if (!url.isValid()) return;

    qDebug() << url;
    const auto &style = Plugin::styles().query(url);
    if (style.empty()) return;

    qDebug() << "matched" << url;

    std::ostringstream oss;
    oss << "<style type=\"text/css\">"
        << style
        << "</style>";
    const auto &html = oss.str();

    auto &&head = m_frame->findFirstElement("head");
    head.appendInside(html.c_str());
}

} // namespace stylist
