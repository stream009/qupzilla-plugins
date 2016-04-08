#ifndef BOOKMARK_DASH_VIEW_H
#define BOOKMARK_DASH_VIEW_H

#include "context_menu.h"
#include "view_p.h"

class QAbstractItemModel;
class QDrag;
class QDragMoveEvent;
class QMenu;
class QModelIndex;
class QMouseEvent;
class QPoint;
class QWidget;

class BrowserWindow;
class BookmarkItem;

namespace bookmark_dash {

template<typename BaseT>
class View : public BaseT, public MenuContext, public view::Slots
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
    bool isMenuItem(const QModelIndex&) override;

    // @override mixin::widget::Drag
    void prepareDrag(QDrag &, const QPoint &) override;

    // @override mixin::widget::Drop
    bool canDrop(const QDragMoveEvent &) override;
    bool canDrop(const QMimeData &) override;
    void onDrop(QDropEvent&) override;

    // @override view::Slot
    void onUrlDropped(
        const QString &title, const QUrl&, const QModelIndex &before) override;

protected:
    BookmarkItem &item(const QModelIndex&) const;

private:
    void setIcon(QAction&, BookmarkItem&);

private:
    BrowserWindow &m_window;
    view::SlotsDelegate m_slotsDelegate;
};

} // namespace bookmark_dash

#include "view.tcc"

#endif // BOOKMARK_DASH_VIEW_H
