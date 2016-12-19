#include "windowadaptor.h"

#include "plugin.h"

#include <QtCore/QObject>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMenuBar>

#include <bookmarkitem.h>
#include <bookmarkstools.h>
#include <browserwindow.h>

namespace bookmark_dash {

WindowAdaptor::
WindowAdaptor(BrowserWindow &window, Plugin &plugin)
    : m_window { window },
      m_plugin { plugin },
      m_menu { *this, plugin.settings() },
      m_toolBar { *this, plugin.settings() },
      m_settingsMenu { plugin.settings() },
      m_iconWatcher { window }
{
    toolsMenu().addMenu(&m_settingsMenu);

    m_remover = new QAction { "&Remove bookmark" };
    m_remover->setShortcut(QKeySequence("Shift+Ctrl+D"));
    this->connect(m_remover, &QAction::triggered,
                  this,      &WindowAdaptor::onBookmarkRemoveRequested);
    assert(m_remover);

    m_window.addAction(m_remover);
}

WindowAdaptor::
~WindowAdaptor()
{
    toolsMenu().removeAction(m_settingsMenu.menuAction());
    m_window.removeAction(m_remover);
}

BrowserWindow &WindowAdaptor::
window() const
{
    return m_window;
}

void WindowAdaptor::
onBookmarkTriggered(BookmarkItem &item)
{
    assert(item.type() == BookmarkItem::Url);

    const auto modifiers = QApplication::keyboardModifiers();
    if (modifiers & Qt::ControlModifier ||
        m_plugin.recentlyPressedButtons() & Qt::MiddleButton)
    {
        BookmarksTools::openBookmarkInNewTab(&m_window, &item);
    }
    else if (modifiers & Qt::ShiftModifier) {
        BookmarksTools::openBookmarkInNewWindow(&item);
    }
    else {
        BookmarksTools::openBookmark(&m_window, &item);
    }
}

void WindowAdaptor::
onBookmarkRemoveRequested()
{
    auto* const view = m_window.weView();
    assert(view);

    assert(mApp);
    auto* const bookmarks = mApp->bookmarks();
    assert(bookmarks);

    auto const& currentUrl = view->url();
    for (auto* const item: bookmarks->searchBookmarks(currentUrl)) {
        bookmarks->removeBookmark(item);
    }
}

QMenu &WindowAdaptor::
toolsMenu() const
{
    auto* const menuBar = m_window.menuBar();
    assert(menuBar);

    const auto &actions = menuBar->actions();
    const auto it = std::find_if(actions.begin(), actions.end(),
        [] (QAction* const action) {
            return action->text() == QObject::tr("&Tools");
        }
    );
    assert(it != actions.end());

    auto* const result = (*it)->menu();
    assert(result);

    return *result;
}

} // namespace bookmark_dash
