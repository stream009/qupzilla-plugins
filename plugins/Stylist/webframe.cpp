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
    this->connect(frame, SIGNAL(javaScriptWindowObjectCleared()),
                  this,  SLOT(slotJavaScriptWindowObjectCleared()));
    this->connect(frame, SIGNAL(loadStarted()),
                  this,  SLOT(slotLoadStarted()));
    this->connect(frame, SIGNAL(loadFinished(bool)),
                  this,  SLOT(slotLoadFinished(bool)));
    this->connect(frame, SIGNAL(pageChanged()),
                  this,  SLOT(slotPageChanged()));

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

void WebFrame::
slotJavaScriptWindowObjectCleared()
{
    qDebug() << __FUNCTION__ << this;
}

void WebFrame::
slotLoadStarted()
{
    qDebug() << __FUNCTION__ << this;
}

void WebFrame::
slotLoadFinished(bool flag)
{
    qDebug() << __FUNCTION__ << flag << this;
}

void WebFrame::
slotPageChanged()
{
    qDebug() << __FUNCTION__ << this;
}

} // namespace stylist
