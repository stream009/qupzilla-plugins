#ifndef BOOKMARK_DASH_MENU_H
#define BOOKMARK_DASH_MENU_H

#include "view.h"

#include <stream9/qt/mixin/menu/tool_tip.hpp>
#include <stream9/qt/mixin/menu_view.hpp>
#include <stream9/qt/mixin/menu_view/drag.hpp>
#include <stream9/qt/mixin/menu_view/drop.hpp>
#include <stream9/qt/mixin/widget/context_menu.hpp>

#include <enhancedmenu.h>

class BookmarkItem;
class BrowserWindow;
class QAbstractItemModel;
class QMenu;
class QModelIndex;
class QWidget;

namespace bookmark_dash {

using MenuBase = View<
                    stream9::qt::mixin::MenuView<
                    stream9::qt::mixin::menu_view::Drag<
                    stream9::qt::mixin::menu_view::Drop<
                    stream9::qt::mixin::menu::ToolTip<
                    stream9::qt::mixin::widget::ContextMenu<
                    ::Menu>>>>>>;

class Menu : public MenuBase
{
    Q_OBJECT
    using Base = MenuBase;
public:
    Menu(BrowserWindow &, QWidget*const parent);

Q_SIGNALS:
    void triggered(BookmarkItem&);

private:
    // @override mixin::MenuView
    QMenu &createSubMenu(QAbstractItemModel &,
                 const QModelIndex &, QWidget* const parent) override;
    void onTriggered(const QModelIndex&) override;

private Q_SLOTS:
    void onMenuMiddleClicked(Menu*);
};

} // namespace bookmark_dash

#endif // BOOKMARK_DASH_MENU_H
