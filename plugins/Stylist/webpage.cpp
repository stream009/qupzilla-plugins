#include "webpage.h"

namespace stylist {

Page::
Page(WebPage* const webPage)
{
    qDebug() << __FUNCTION__;
    this->connect(webPage, SIGNAL(frameCreated(QWebFrame*)),
                  this,    SLOT(slotFrameCreated(QWebFrame*)));
    slotFrameCreated(webPage->mainFrame());
}

void Page::
slotFrameCreated(QWebFrame* frame) noexcept
{
    qDebug() << __FUNCTION__ << frame;
    m_frames.emplace_back(new WebFrame { frame });
}

} // namespace stylist
