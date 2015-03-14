#ifndef BOOKMARK_DASH_MENU_H
#define BOOKMARK_DASH_MENU_H

#include "view.h"

#include "mixin/menu/tool_tip.hpp"
#include "mixin/menu_view.hpp"
#include "mixin/menu_view/drag.hpp"
#include "mixin/menu_view/drop.hpp"
#include "mixin/widget/context_menu.hpp"

#include <enhancedmenu.h>

class BrowserWindow;
class QAbstractItemModel;
class QMenu;
class QModelIndex;
class QWidget;

namespace bookmark_dash {

using MenuBase = View<
                    mixin::MenuView<
                    mixin::menu_view::Drag<
                    mixin::menu_view::Drop<
                    mixin::menu::ToolTip<
                    mixin::widget::ContextMenu<
                    ::Menu>>>>>>;

class Menu : public MenuBase
{
    Q_OBJECT
    using Base = MenuBase;
public:
    Menu(BrowserWindow &, QWidget*const parent);

private:
    // @override mixin::MenuView
    QMenu &createSubMenu(QAbstractItemModel &,
                 const QModelIndex &, QWidget* const parent) override;
    void onTriggered(const QModelIndex&) override;

private Q_SLOTS:
    void onMenuMiddleClicked(Menu*);
};

} // namespace bookmark_dash

#include <toolbutton.h>

class MenuButton;
class QWidget;

namespace bookmark_dash {

class MenuButton : public ToolButton
{
    using Base = ToolButton;
public:
    MenuButton(QWidget* const parent = nullptr);
};

} // namespace bookmark_dash

#endif // BOOKMARK_DASH_MENU_H
