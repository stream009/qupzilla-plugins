#include "menu.h"

#include <QtCore/QAbstractItemModel>
#include <QtCore/QModelIndex>
#include <QtGui/QMenu>
#include <QtGui/QMenu>
#include <QtGui/QWidget>

#include <browserwindow.h>

namespace bookmark_dash {

Menu::
Menu(BrowserWindow &window, QWidget* const parent)
    : Base { window, parent }
{
    this->connect(this, SIGNAL(menuMiddleClicked(Menu*)),
                  this, SLOT(onMenuMiddleClicked(Menu*)));
}

QMenu &Menu::
createSubMenu(QAbstractItemModel &model,
              const QModelIndex &root, QWidget* const parent)
{
    auto* const menu = new Menu { this->window(), parent };
    assert(menu);
    menu->setModel(model, root);

    return *menu;
}

void Menu::
onTriggered(const QModelIndex &index)
{
    assert(index.isValid());
    auto &item = this->item(index);
    assert(item.type() == BookmarkItem::Url);

    const auto modifiers = QApplication::keyboardModifiers();
    if (modifiers & Qt::ControlModifier ||
        this->recentlyPressedButtons() & Qt::MiddleButton)
    {
        openBookmarkInNewTab(item);
    }
    else if (modifiers & Qt::ShiftModifier) {
        openBookmarkInNewWindow(item);
    }
    else {
        openBookmark(item);
    }
}

void Menu::
onMenuMiddleClicked(Menu* const menu)
{
    assert(menu);

    auto* const action = menu->menuAction();
    assert(action);
    const auto &index = this->index(*action);
    assert(index.isValid());
    auto &item = this->item(index);

    openFolderInTabs(item);
}

} // namespace bookmark_dash

#include <QtGui/QWidget>

namespace bookmark_dash {

MenuButton::
MenuButton(QWidget* const parent/*= 0*/)
    : Base { parent }
{
    this->setObjectName("navigation-button-bookmarks-menu");
    this->setPopupMode(QToolButton::InstantPopup);
    this->setToolbarButtonLook(true);
    this->setToolTip(tr("Bookmarks Menu"));
    this->setAutoRaise(true);
    this->setFocusPolicy(Qt::NoFocus);
    this->setShowMenuInside(true);
}

} // namespace bookmark_dash

