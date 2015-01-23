#ifndef STYLIST_WEBPAGE_H
#define STYLIST_WEBPAGE_H

#include "webframe.h"

#include <../webkit/webpage.h>

#include <memory>
#include <unordered_map>

#include <QtCore/QDebug>
#include <QtCore/QObject>

namespace stylist {

class Page : QObject
{
    Q_OBJECT
public:
    explicit Page(WebPage* const webPage);

private Q_SLOTS:
    void slotFrameCreated(QWebFrame* frame) noexcept;
    void slotFrameDestroyed();

private:
    std::unordered_map<QObject*, std::unique_ptr<WebFrame>> m_frames;
};

} // namespace stylist

#endif // STYLIST_WEBPAGE_H
