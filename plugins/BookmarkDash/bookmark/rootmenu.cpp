#include "rootmenu.h"

#include "bookmark_dialog.h"

#include <QtCore/QAbstractItemModel>
#include <QtGui/QAction>
#include <QtGui/QWidget>

#include <bookmarks.h>
#include <bookmarkstools.h>
#include <browserwindow.h>
#include <browsinglibrary.h>
#include <tabbedwebview.h>

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
    auto* const webView = m_window.weView();
    assert(webView);

    const auto &url = webView->url();
    assert(mApp);
    auto* const bookmarks = mApp->bookmarks();
    assert(bookmarks);

    const auto &items = bookmarks->searchBookmarks(url);
    if (!items.empty()) {
        auto* const item = items.front();
        assert(item);
        BookmarkDialog dialog { *item, m_window };
        dialog.setWindowTitle(tr("Bookmark Property"));

        const auto result = dialog.exec();
        if (result == QDialog::Accepted) {
            bookmarks->changeBookmark(item);
        }
    }
    else {
        BookmarksTools::addBookmarkDialog(
                                   &m_window, url, webView->title());
    }
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
