#include "menu.h"

#include <QtCore/QAbstractItemModel>
#include <QtCore/QModelIndex>
#include <QtGui/QMenu>
#include <QtGui/QMenu>
#include <QtGui/QWidget>

#include <bookmarkitem.h>
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

    Q_EMIT triggered(item);
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

    this->openFolderInTabs(item);
}

} // namespace bookmark_dash
