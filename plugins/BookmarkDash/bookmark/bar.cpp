#include "bar.h"

#include "menu.h"

#include <QtCore/QAbstractItemModel>
#include <QtCore/QDebug>
#include <QtCore/QModelIndex>
#include <QtCore/QSize>
#include <QtGui/QFontMetrics>
#include <QtGui/QIcon>
#include <QtGui/QWidget>

#include <browserwindow.h>

namespace bookmark_dash {

Bar::
Bar(BrowserWindow &window, QWidget* const parent)
: Base { window, parent }
{
    QFontMetrics fm { this->font() };
    this->setIconSize(QSize { fm.height(), fm.height() });
    this->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
}

void Bar::
onActionTriggered(const QModelIndex &index)
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

QMenu &Bar::
createSubMenu(QAbstractItemModel &model,
              const QModelIndex &root, QWidget* const parent)
{
    auto* const menu = new Menu { this->window(), parent };
    assert(menu);
    menu->setModel(model, root);

    return *menu;
}

} // namespace bookmark_dash
