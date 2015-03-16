#ifndef BOOKMARK_DASH_VIEW_H
#define BOOKMARK_DASH_VIEW_H

#include "context_menu.h"

class QAbstractItemModel;
class QMenu;
class QModelIndex;
class QMouseEvent;
class QPoint;
class QWidget;

class BrowserWindow;
class BookmarkItem;

namespace bookmark_dash {

template<typename BaseT>
class View : public BaseT, public MenuContext
{
    using Base = BaseT;
public:
    View(BrowserWindow &, QWidget* const parent);

    // @overload mixin::ActionView
    BookmarksModel &model() const override;

    // @override MenuContext
    BrowserWindow &window() const override { return m_window; }
    BookmarkItem *currentItem() const override;
    BookmarkItem &parentItem() const override;
    void openFolderInTabs(BookmarkItem&) override;

protected:
    // @override mixin::ActionView
    QAction &createItemAction(const QModelIndex&) override;

protected:
    BookmarkItem &item(const QModelIndex&) const;

private:
    BrowserWindow &m_window;
};

} // namespace bookmark_dash

#include "view.tcc"

#endif // BOOKMARK_DASH_VIEW_H
