#ifndef STYLIST_WEBPAGE_H
#define STYLIST_WEBPAGE_H

#include "webframe.h"

#include <../webkit/webpage.h>

#include <memory>
#include <vector>

#include <QtCore/QDebug>
#include <QtCore/QObject>

namespace stylist {

class Page : QObject
{
    Q_OBJECT
public:
    Page(WebPage* const webPage)
    {
        qDebug() << __FUNCTION__;
        this->connect(webPage, SIGNAL(frameCreated(QWebFrame*)),
                      this,    SLOT(slotFrameCreated(QWebFrame*)));
        slotFrameCreated(webPage->mainFrame());
    }

private slots:
    void slotFrameCreated(QWebFrame* frame) noexcept
    {
        qDebug() << __FUNCTION__ << frame;
        m_frames.emplace_back(new WebFrame { frame });
    }

private:
    std::vector<std::unique_ptr<WebFrame>> m_frames;
};

} // namespace stylist

#endif // STYLIST_WEBPAGE_H
