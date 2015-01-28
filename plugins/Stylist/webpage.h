#ifndef STYLIST_WEBPAGE_H
#define STYLIST_WEBPAGE_H

#include <../webkit/webpage.h>

#include <memory>
#include <unordered_map>

#include <QtCore/QDebug>
#include <QtCore/QObject>

namespace stylist {

class WebFrame;

class Page : QObject
{
    Q_OBJECT
public:
    explicit Page(WebPage* const webPage);
    virtual ~Page();

private Q_SLOTS:
    void slotFrameCreated(QWebFrame* frame) noexcept;
    void slotFrameDestroyed();

private:
    std::unordered_map<QObject*, std::unique_ptr<WebFrame>> m_frames;
};

} // namespace stylist

#endif // STYLIST_WEBPAGE_H
