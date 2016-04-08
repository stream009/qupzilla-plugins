#ifndef BOOKMARK_DASH_BAR_H
#define BOOKMARK_DASH_BAR_H

#include "view.h"

#include "mixin/tool_bar_view.hpp"
#include "mixin/tool_bar_view/drag.hpp"
#include "mixin/tool_bar_view/drop.hpp"
#include "mixin/widget/context_menu.hpp"

#include <QtWidgets/QToolBar>

class BookmarkItem;
class BrowserWindow;
class QAbstractItemModel;
class QModelIndex;
class QWidget;

namespace bookmark_dash {

using BarBase = View<
                    mixin::ToolBarView<
                    mixin::tool_bar_view::Drag<
                    mixin::tool_bar_view::Drop<
                    mixin::widget::ContextMenu<
                    QToolBar>>>>>;

class Bar : public BarBase
{
    Q_OBJECT
    using Base = BarBase;
public:
    Bar(BrowserWindow &, QWidget* const parent);

Q_SIGNALS:
    void triggered(BookmarkItem&);

private:
    // @override mixin::ToolBarView
    void onActionTriggered(const QModelIndex&) override;
    QMenu &createSubMenu(QAbstractItemModel &,
                    const QModelIndex &, QWidget* const parent) override;
};

} // namespace bookmark_dash

#endif // BOOKMARK_DASH_BAR_H
