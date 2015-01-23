#include "webpage.h"

#include <boost/make_unique.hpp>

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

    this->connect(frame, SIGNAL(destroyed()),
                  this,  SLOT(slotFrameDestroyed()));

    m_frames.emplace(frame, boost::make_unique<WebFrame>(frame));
}

void Page::
slotFrameDestroyed()
{
    auto* const frame = this->sender();
    qDebug() << __FUNCTION__ << frame;

    assert(m_frames.count(frame) == 1); //TODO better
    m_frames.erase(frame);
}

} // namespace stylist
