#include "menuadaptor.h"

#include "bookmark/rootmenu.h"
#include "bookmark/menubutton.h"
#include "settings.h"
#include "windowadaptor.h"

#include <cassert>

#include <QtCore/QDebug>
#include <QtGui/QBoxLayout>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>

#include <bookmarks.h>
#include <bookmarksmenu.h>
#include <browserwindow.h>
#include <navigationbar.h>

namespace bookmark_dash {

MenuAdaptor::
MenuAdaptor(WindowAdaptor &window, Settings &settings)
    : m_window { window.window() }
{
    saveOriginal();

    m_menu.reset(new RootMenu { m_window, nullptr });
    assert(m_menu);

    this->connect(m_menu.get(), SIGNAL(triggered(BookmarkItem&)),
                  &window,        SLOT(onBookmarkTriggered(BookmarkItem&)));

    m_menuButton.reset(new MenuButton);
    assert(m_menuButton);
    m_menuButton->setMenu(m_menu.get());

    install();

    const auto &setting = settings.bookmarksMenu();
    this->connect(&setting, SIGNAL(changed(bool)),
                  this,        SLOT(install(bool)));

    assert(m_original);
    assert(m_menu);
    assert(m_menuButton);
}

MenuAdaptor::
~MenuAdaptor()
{
    uninstall();
}

void MenuAdaptor::
install(const bool flag)
{
    if (flag) {
        install();
    }
    else {
        uninstall();
    }
}

void MenuAdaptor::
install()
{
    assert(m_original);
    assert(m_menu);

    if (m_installed) return;

    auto* const menuAction = m_original->menuAction();
    assert(menuAction);

    // Install the root menu to menu bar
    auto* const menuBar = m_window.menuBar();
    assert(menuBar);
    menuBar->insertMenu(menuAction, m_menu.get());
    menuBar->removeAction(menuAction);

    // Install the root menu to super menu
    auto* const superMenu = m_window.superMenu();
    assert(superMenu);
    superMenu->insertMenu(menuAction, m_menu.get());
    superMenu->removeAction(menuAction);

    // Install the menu button to navigation bar
    auto* const navigationBar = m_window.navigationBar();
    assert(navigationBar);
    auto* const before =
        navigationBar->findChild<ToolButton*>("navigation-button-supermenu");
    assert(before);
    auto* const layout = dynamic_cast<QBoxLayout*>(navigationBar->layout());
    assert(layout);
    const auto index = layout->indexOf(before);
    layout->insertWidget(index, m_menuButton.get());

    // Install root menu's shortcut actions to the window
    for (auto* const action: m_menu->actions()) {
        assert(action);
        if(!action->shortcuts().empty()) {
            m_window.addAction(action);
            m_installedShortcutActions.push_back(action);
        }
    }

    // Backup original shortcuts then remove them.
    QList<QKeySequence> emptyShortcuts;
    for (auto* const action: m_original->actions()) {
        if (!action->shortcuts().empty()) {
            m_shortcutBackup.emplace(action, action->shortcuts());
            action->setShortcuts(emptyShortcuts);
        }
    }

    m_installed = true;
}

void MenuAdaptor::
uninstall()
{
    assert(m_menu);
    assert(m_original);

    if (!m_installed) return;

    auto* const menuAction = m_menu->menuAction();
    assert(menuAction);

    // Uninstall from menu bar
    auto* const menuBar = m_window.menuBar();
    assert(menuBar);
    menuBar->insertMenu(menuAction, m_original);
    menuBar->removeAction(menuAction);

    // Uninstall from super menu
    auto* const superMenu = m_window.superMenu();
    assert(superMenu);
    superMenu->insertMenu(menuAction, m_original);
    superMenu->removeAction(menuAction);

    // Uninstall menu button
    assert(m_menuButton);
    auto* const parent = m_menuButton->parentWidget();
    assert(parent);
    auto* const layout = parent->layout();
    assert(layout);
    layout->removeWidget(m_menuButton.get());

    // Uninstall shortcut actions from window
    for (auto* const action: m_installedShortcutActions) {
        assert(action);
        m_window.removeAction(action);
    }

    // Restore preserved shortcuts of original menu actions
    for (const auto &item: m_shortcutBackup) {
        auto* const action = item.first;
        const auto &shortcuts = item.second;

        action->setShortcuts(shortcuts);
    }

    m_installed = false;
}

void MenuAdaptor::
saveOriginal()
{
    auto* const menuBar = m_window.menuBar();
    assert(menuBar);
    const auto &actions = menuBar->actions();
    const auto it = std::find_if(
        actions.begin(), actions.end(),
        [] (QAction* const action) {
            return dynamic_cast<BookmarksMenu*>(action->menu());
        }
    );
    assert(it != actions.end());
    m_original = static_cast<QMenu*>((*it)->menu());
    assert(m_original);
}

} // namespace bookmark_dash
