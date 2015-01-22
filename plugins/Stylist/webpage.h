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
    explicit Page(WebPage* const webPage);

private slots:
    void slotFrameCreated(QWebFrame* frame) noexcept;

private:
    std::vector<std::unique_ptr<WebFrame>> m_frames;
};

} // namespace stylist

#endif // STYLIST_WEBPAGE_H
