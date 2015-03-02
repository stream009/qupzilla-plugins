#include "plugin/windowadaptor.h"

#include "bookmark/menudash.h"

#include <browserwindow.h>
#include <bookmarksmenu.h>

#include <QtGui/QMenuBar>

namespace bookmark_dash {

WindowAdaptor::
WindowAdaptor(BrowserWindow &window)
    : m_window { window }
{
    auto* const menuBar = window.menuBar();
    assert(menuBar);
    const auto &actions = menuBar->actions();
    const auto it = std::find_if(
        actions.begin(), actions.end(),
        [] (QAction* const action) {
            return dynamic_cast<BookmarksMenu*>(action->menu());
        }
    );
    assert(it != actions.end());
    m_originalMenu = static_cast<BookmarksMenu*>((*it)->menu());
    assert(m_originalMenu);

    auto* const parent = m_originalMenu->parentWidget(); // could be nullptr

    m_menu.reset(new MenuDash { nullptr });
    assert(m_menu);

    auto* const menuAction = m_originalMenu->menuAction();
    assert(menuAction);
    menuBar->insertMenu(menuAction, m_menu.get());
    menuBar->removeAction(menuAction);
}

WindowAdaptor::
~WindowAdaptor()
{
    if (!m_menu) return;

    auto* const menuAction = m_menu->menuAction();
    assert(menuAction);

    auto* const menuBar = m_window.menuBar();
    assert(menuBar);
    menuBar->insertMenu(menuAction, m_originalMenu);
    menuBar->removeAction(menuAction);
}

} // namespace bookmark_dash
