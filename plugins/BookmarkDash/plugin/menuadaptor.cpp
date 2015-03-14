#include "menuadaptor.h"

#include "bookmark/menu.h"
#include "settings.h"

#include <cassert>

#include <QtCore/QDebug>
#include <QtGui/QBoxLayout>

#include <bookmarks.h>
#include <bookmarksmenu.h>
#include <bookmarksmodel.h>
#include <bookmarkstools.h>
#include <browserwindow.h>
#include <mainapplication.h>
#include <navigationbar.h>

namespace bookmark_dash {

MenuAdaptor::
MenuAdaptor(BrowserWindow &window, Settings &settings)
    : m_window { window }
{
    saveOriginal();
    createTopMenu();
    assert(m_menu);

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
bookmarkPage()
{
    //qDebug() << __func__;
    m_window.bookmarkPage();
}

void MenuAdaptor::
bookmarkAllTabs()
{
    //qDebug() << __func__;
    BookmarksTools::bookmarkAllTabsDialog(&m_window, m_window.tabWidget());
}

void MenuAdaptor::
showBookmarksManager()
{
    //qDebug() << __func__;
    assert(mApp);
    auto* const browsingLibrary = mApp->browsingLibrary();
    assert(browsingLibrary);

    browsingLibrary->showBookmarks(&m_window);
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

    // Install to menu bar
    auto* const menuBar = m_window.menuBar();
    assert(menuBar);
    menuBar->insertMenu(menuAction, m_menu.get());
    menuBar->removeAction(menuAction);

    // Install to super menu
    auto* const superMenu = m_window.superMenu();
    assert(superMenu);
    superMenu->insertMenu(menuAction, m_menu.get());
    superMenu->removeAction(menuAction);

    // Install menu button to navigation bar
    auto* const navigationBar = m_window.navigationBar();
    assert(navigationBar);
    auto* const before =
        navigationBar->findChild<ToolButton*>("navigation-button-supermenu");
    assert(before);
    auto* const layout = dynamic_cast<QBoxLayout*>(navigationBar->layout());
    assert(layout);
    const auto index = layout->indexOf(before);
    layout->insertWidget(index, m_menuButton.get());

    // Install shortcut actions
    assert(m_bookmarkThisPage);
    assert(m_organizeBookmarks);
    m_window.addAction(m_bookmarkThisPage);
    m_window.addAction(m_organizeBookmarks);

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

    // Uninstall shortcut actions
    assert(m_bookmarkThisPage);
    assert(m_organizeBookmarks);
    m_window.removeAction(m_bookmarkThisPage);
    m_window.removeAction(m_organizeBookmarks);

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

void MenuAdaptor::
createTopMenu()
{
    m_menu.reset(new Menu { m_window, nullptr });
    assert(m_menu);

    m_menu->setTitle(tr("&Bookmarks"));

    auto* action = new QAction { tr("Bookmark &This Page"), this };
    assert(action);
    action->setIcon(QIcon::fromTheme("bookmark-new"));
    action->setShortcut(QKeySequence("Ctrl+D"));
    this->connect(action, SIGNAL(triggered()),
                  this,     SLOT(bookmarkPage()));
    m_menu->addAction(action);
    m_bookmarkThisPage = action;

    action = new QAction { tr("Bookmark &All Tabs"), this };
    assert(action);
    action->setIcon(QIcon::fromTheme("bookmark-new-list"));
    this->connect(action, SIGNAL(triggered()),
                  this,     SLOT(bookmarkAllTabs()));
    m_menu->addAction(action);

    action = new QAction { tr("Organize &Bookmarks"), this };
    assert(action);
    action->setIcon(QIcon::fromTheme("bookmarks-organize"));
    action->setShortcut(QKeySequence("Ctrl+Shift+O"));
    this->connect(action, SIGNAL(triggered()),
                  this,     SLOT(showBookmarksManager()));
    m_menu->addAction(action);
    m_organizeBookmarks = action;

    m_menu->addSeparator();

    assert(mApp);
    auto* const bookmarks = mApp->bookmarks();
    assert(bookmarks);
    auto* const menuFolder = bookmarks->menuFolder();
    assert(menuFolder);

    auto* const model = bookmarks->model();
    assert(model);
    const auto &root = model->index(menuFolder);
    assert(root.isValid());

    m_menu->setModel(*model, root);
}

} // namespace bookmark_dash
