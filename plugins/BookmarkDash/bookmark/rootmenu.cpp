#include "rootmenu.h"

#include <QtCore/QAbstractItemModel>
#include <QtGui/QAction>
#include <QtGui/QWidget>

#include <bookmarks.h>
#include <bookmarkstools.h>
#include <browserwindow.h>
#include <browsinglibrary.h>

namespace bookmark_dash {

RootMenu::
RootMenu(BrowserWindow &window, QWidget* parent)
    : Base { window, parent },
      m_window { window }
{
    this->setTitle(tr("&Bookmarks"));

    auto* action = new QAction { tr("Bookmark &This Page"), this };
    assert(action);
    action->setIcon(QIcon::fromTheme("bookmark-new"));
    action->setShortcut(QKeySequence("Ctrl+D"));
    this->connect(action, SIGNAL(triggered()),
                  this,     SLOT(onBookmarkThisPage()));
    this->addAction(action);

    action = new QAction { tr("Bookmark &All Tabs"), this };
    assert(action);
    action->setIcon(QIcon::fromTheme("bookmark-new-list"));
    this->connect(action, SIGNAL(triggered()),
                  this,     SLOT(onBookmarkAllTabs()));
    this->addAction(action);

    action = new QAction { tr("Organize &Bookmarks"), this };
    assert(action);
    action->setIcon(QIcon::fromTheme("bookmarks-organize"));
    action->setShortcut(QKeySequence("Ctrl+Shift+O"));
    this->connect(action, SIGNAL(triggered()),
                  this,     SLOT(onOrganizeBookmarks()));
    this->addAction(action);

    this->addSeparator();

    assert(mApp);
    auto* const bookmarks = mApp->bookmarks();
    assert(bookmarks);
    auto* const menuFolder = bookmarks->menuFolder();
    assert(menuFolder);

    auto* const model = bookmarks->model();
    assert(model);
    const auto &root = model->index(menuFolder);
    assert(root.isValid());

    this->setModel(*model, root);
}

void RootMenu::
onBookmarkThisPage()
{
    //qDebug() << __func__;
    m_window.bookmarkPage();
}

void RootMenu::
onBookmarkAllTabs()
{
    //qDebug() << __func__;
    BookmarksTools::bookmarkAllTabsDialog(&m_window, m_window.tabWidget());
}

void RootMenu::
onOrganizeBookmarks()
{
    //qDebug() << __func__;
    assert(mApp);
    auto* const browsingLibrary = mApp->browsingLibrary();
    assert(browsingLibrary);

    browsingLibrary->showBookmarks(&m_window);
}

} // namespace bookmark_dash
