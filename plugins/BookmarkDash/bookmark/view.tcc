#include "view.h"

#include <bookmarkitem.h>
#include <bookmarksmodel.h>
#include <bookmarkstools.h>
#include <browserwindow.h>

#include <QtCore/QAbstractItemModel>
#include <QtCore/QDebug>
#include <QtCore/QModelIndex>
#include <QtGui/QApplication>
#include <QtGui/QDrag>
#include <QtGui/QFontMetrics>
#include <QtGui/QIcon>
#include <QtGui/QMenu>
#include <QtGui/QMouseEvent>
#include <QtGui/QWidget>

namespace bookmark_dash {

template<typename BaseT>
View<BaseT>::
View(BrowserWindow &window, QWidget*const parent)
    : Base { parent },
      m_window { window }
{
    auto* const contextMenu =
                new bookmark_dash::ContextMenu { *this, nullptr };
    assert(contextMenu);
    this->setContextMenu(*contextMenu);
}

template<typename BaseT>
inline void View<BaseT>::
openBookmark(BookmarkItem &item)
{
    BookmarksTools::openBookmark(&m_window, &item);
}

template<typename BaseT>
inline void View<BaseT>::
openBookmarkInNewTab(BookmarkItem &item)
{
    BookmarksTools::openBookmarkInNewTab(&m_window, &item);
}

template<typename BaseT>
inline void View<BaseT>::
openBookmarkInNewWindow(BookmarkItem &item)
{
    BookmarksTools::openBookmarkInNewWindow(&item);
}

template<typename BaseT>
inline void View<BaseT>::
openFolderInTabs(BookmarkItem &item)
{
    BookmarksTools::openFolderInTabs(&m_window, &item);
}

template<typename BaseT>
inline BookmarkItem *View<BaseT>::
currentItem() const
{
    if (this->currentIndex().isValid()) {
        return &item(this->currentIndex());
    }
    else {
        return nullptr;
    }
}

template<typename BaseT>
inline BookmarkItem &View<BaseT>::
parentItem() const
{
    auto &rootIndex = this->rootIndex();
    assert(rootIndex.isValid());

    return item(rootIndex);
}

template<typename BaseT>
inline QAction &View<BaseT>::
createItemAction(const QModelIndex &index)
{
    assert(index.isValid());
    assert(index.model() == &this->model());
    assert(index.parent() == this->rootIndex());

    auto &item = this->item(index);

    auto &result = Base::createItemAction(index);
    if (item.isSeparator()) {
        result.setSeparator(true);
    }

    const auto &title = item.title();
    const QFontMetrics fm { result.font() };
    const auto &elidedText = fm.elidedText(title, Qt::ElideRight, 250);
    result.setText(elidedText);

    const auto &icon = item.icon();
    result.setIcon(icon);

    return result;
}

template<typename BaseT>
inline BookmarksModel &View<BaseT>::
model() const
{
    return dynamic_cast<BookmarksModel&>(Base::model());
}

template<typename BaseT>
inline void View<BaseT>::
mousePressEvent(QMouseEvent* const event)
{
    assert(event);
    Base::mousePressEvent(event);

    m_recentlyPressedButtons = event->buttons();
}

template<typename BaseT>
inline BookmarkItem &View<BaseT>::
item(const QModelIndex &index) const
{
    assert(index.isValid());

    auto* const result = model().item(index);
    assert(result);

    return *result;
}

template<typename BaseT>
Qt::MouseButtons View<BaseT>::
recentlyPressedButtons() const
{
    return m_recentlyPressedButtons;
}

} // namespace bookmark_dash

