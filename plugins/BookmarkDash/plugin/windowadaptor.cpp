#include "plugin/windowadaptor.h"

#include "bookmark/bar.h"
#include "bookmark/menu.h"

#include <bookmarks.h>
#include <bookmarksmenu.h>
#include <bookmarksmodel.h>
#include <bookmarkstoolbar.h>
#include <bookmarkstools.h>
#include <browserwindow.h>
#include <browsinglibrary.h>
#include <mainapplication.h>

#include <QtCore/QDebug>
#include <QtGui/QBoxLayout>
#include <QtGui/QMenuBar>

namespace bookmark_dash {

WindowAdaptor::
WindowAdaptor(BrowserWindow &window)
    : m_window { window }
{
    saveOriginalMenu();
    createMenu();
    setModelToMenu();
    installMenu();

    m_originalBar = m_window.bookmarksToolbar();
    assert(m_originalBar);
    auto* const container = m_originalBar->parentWidget();
    assert(container);
    auto* const layout = dynamic_cast<QBoxLayout*>(container->layout());
    assert(layout);

    m_bar.reset(new Bar { m_window, nullptr });

    layout->insertWidget(layout->indexOf(m_originalBar), m_bar.get());
    m_originalBar->hide();
    layout->removeWidget(m_originalBar);
    container->update();

    assert(mApp);
    auto* const bookmarks = mApp->bookmarks();
    assert(bookmarks);
    auto* const toolBarFolder = bookmarks->toolbarFolder();
    assert(toolBarFolder);

    auto* const model = bookmarks->model();
    assert(model);
    const auto &root = model->index(toolBarFolder);
    assert(root.isValid());

    m_bar->setModel(*model, root);
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

    if (!m_bar) return;
    assert(m_originalBar);

    auto* const container = m_bar->parentWidget();
    assert(container);
    auto* const layout = dynamic_cast<QBoxLayout*>(container->layout());
    assert(layout);

    layout->insertWidget(layout->indexOf(m_bar.get()), m_originalBar);
    layout->removeWidget(m_bar.get());
    m_originalBar->show();
    container->update();
}

void WindowAdaptor::
saveOriginalMenu()
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
    m_originalMenu = static_cast<BookmarksMenu*>((*it)->menu());
    assert(m_originalMenu);
}

void WindowAdaptor::
createMenu()
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

    m_menu->addSeparator();
}

void WindowAdaptor::
setModelToMenu()
{
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

void WindowAdaptor::
installMenu()
{
    assert(m_originalMenu);
    assert(m_menu);

    auto* const menuAction = m_originalMenu->menuAction();
    assert(menuAction);
    auto* const menuBar = m_window.menuBar();
    assert(menuBar);
    menuBar->insertMenu(menuAction, m_menu.get());
    menuBar->removeAction(menuAction);
}

void WindowAdaptor::
bookmarkPage()
{
    //qDebug() << __func__;
    m_window.bookmarkPage();
}

void WindowAdaptor::
bookmarkAllTabs()
{
    //qDebug() << __func__;
    BookmarksTools::bookmarkAllTabsDialog(&m_window, m_window.tabWidget());
}

void WindowAdaptor::
showBookmarksManager()
{
    //qDebug() << __func__;
    assert(mApp);
    auto* const browsingLibrary = mApp->browsingLibrary();
    assert(browsingLibrary);

    browsingLibrary->showBookmarks(&m_window);
}

} // namespace bookmark_dash
