#include "context_menu.h"

#include "bookmark_dialog.h"
#include "folder_dialog.h"

#include <bookmarkitem.h>
#include <bookmarks.h>
#include <bookmarksmodel.h>
#include <browserwindow.h>
#include <iconprovider.h>
#include <mainapplication.h>

#include <cassert>
#include <memory>

namespace bookmark_dash {

ContextMenu::
ContextMenu(MenuContext &context, QWidget* const parent)
    : Base { parent },
      m_context { context },
      m_urlActions { this },
      m_folderActions { this }
{
    auto* action = new QAction { tr("&Open"), &m_urlActions };
    this->connect(action, SIGNAL(triggered()),
                  this,     SLOT(onOpen()));
    this->addAction(action);

    action = new QAction { tr("Open in a Ne&w Tab"), &m_urlActions };
    action->setIcon(IconProvider::newTabIcon());
    this->connect(action, SIGNAL(triggered()),
                  this,     SLOT(onOpenInNewTab()));
    this->addAction(action);

    action = new QAction { tr("Open in a New &Window"), &m_urlActions };
    action->setIcon(IconProvider::newWindowIcon());
    this->connect(action, SIGNAL(triggered()),
                  this,     SLOT(onOpenInNewWindow()));
    this->addAction(action);

    action = new QAction { tr("&Open All in Tabs"), &m_folderActions };
    action->setIcon(IconProvider::newTabIcon());
    this->connect(action, SIGNAL(triggered()),
                  this,     SLOT(onOpenAllInTabs()));
    this->addAction(action);

    this->addSeparator();

    action = new QAction { tr("New &Bookmark"), this };
    this->connect(action, SIGNAL(triggered()),
                  this,     SLOT(onNewBookmark()));
    this->addAction(action);

    action = new QAction { tr("New &Folder"), this };
    this->connect(action, SIGNAL(triggered()),
                  this,     SLOT(onNewFolder()));
    this->addAction(action);

    action = new QAction { tr("New &Separator"), this };
    this->connect(action, SIGNAL(triggered()),
                  this,     SLOT(onNewSeparator()));
    this->addAction(action);

    this->addSeparator();
#if 0 //TODO
    action = new QAction { tr("Cu&t"), this };
    action->setIcon(QIcon::fromTheme("edit-cut"));
    this->connect(action, SIGNAL(triggered()),
                  this,     SLOT(onCut()));
    this->addAction(action);

    action = new QAction { tr("&Paste"), this };
    action->setIcon(QIcon::fromTheme("edit-paste"));
    this->connect(action, SIGNAL(triggered()),
                  this,     SLOT(onPaste()));
    this->addAction(action);

    this->addSeparator();
#endif
    action = new QAction { tr("&Delete"), this };
    action->setIcon(QIcon::fromTheme("edit-delete"));
    this->connect(action, SIGNAL(triggered()),
                  this,     SLOT(onDelete()));
    this->addAction(action);

    this->addSeparator();

    m_propertyAction.setText(tr("&Property"));
    this->connect(&m_propertyAction, SIGNAL(triggered()),
                  this,                SLOT(onProperty()));
    this->addAction(&m_propertyAction);

    this->connect(this, SIGNAL(aboutToShow()),
                  this,   SLOT(onAboutToShow()));
}

void ContextMenu::
onAboutToShow()
{
    const auto* const item = m_context.currentItem();
    if (!item) {
        m_urlActions.setVisible(false);
        m_folderActions.setVisible(false);
        m_propertyAction.setVisible(false);
    }
    else {
        switch (item->type()) {
        case BookmarkItem::Url:
            m_urlActions.setVisible(true);
            m_folderActions.setVisible(false);
            m_propertyAction.setVisible(true);
            break;
        case BookmarkItem::Folder:
            m_urlActions.setVisible(false);
            m_folderActions.setVisible(true);
            m_propertyAction.setVisible(true);
            break;
        default:
            m_urlActions.setVisible(false);
            m_folderActions.setVisible(false);
            m_propertyAction.setVisible(false);
            break;
        }
    }
}

void ContextMenu::
onOpen()
{
    auto* const item = m_context.currentItem();
    assert(item);
    assert(item->type() == BookmarkItem::Url);
    m_context.openBookmark(*item);
}

void ContextMenu::
onOpenInNewTab()
{
    auto* const item = m_context.currentItem();
    assert(item);
    assert(item->type() == BookmarkItem::Url);
    m_context.openBookmarkInNewTab(*item);
}

void ContextMenu::
onOpenInNewWindow()
{
    auto* const item = m_context.currentItem();
    assert(item);
    assert(item->type() == BookmarkItem::Url);
    m_context.openBookmarkInNewWindow(*item);
}

void ContextMenu::
onOpenAllInTabs()
{
    auto* const item = m_context.currentItem();
    assert(item);
    assert(item->type() == BookmarkItem::Folder);
    m_context.openFolderInTabs(*item);
}

void ContextMenu::
onNewBookmark()
{
    auto* const newItem = new BookmarkItem { BookmarkItem::Url };
    assert(newItem);
    BookmarkDialog dialog { *newItem, m_context.window() };
    dialog.setWindowTitle(tr("New Bookmark"));

    const auto result = dialog.exec();
    if (result == QDialog::Accepted) {
        // BookmarkModel take ownership of the item.
        insertBookmark(*newItem);
    }
    else {
        delete newItem;
    }
}

void ContextMenu::
onNewFolder()
{
    auto* const newItem = new BookmarkItem { BookmarkItem::Folder };
    assert(newItem);
    FolderDialog dialog { *newItem, m_context.window() };
    dialog.setWindowTitle(tr("New Folder"));

    const auto result = dialog.exec();
    if (result == QDialog::Accepted) {
        // BookmarkModel take ownership of the item.
        insertBookmark(*newItem);
    }
    else {
        delete newItem;
    }
}

void ContextMenu::
onNewSeparator()
{
    auto* const newItem = new BookmarkItem { BookmarkItem::Separator };
    assert(newItem);

    // BookmarkModel takes ownership of the item.
    insertBookmark(*newItem);
}

void ContextMenu::
onCut()
{
    //TODO
}

void ContextMenu::
onPaste()
{
    //TODO
}

void ContextMenu::
onDelete()
{
    auto* const item = m_context.currentItem();
    bookmarks().removeBookmark(item);
}

void ContextMenu::
onProperty()
{
    auto* const item = m_context.currentItem();
    assert(item->type() == BookmarkItem::Url ||
           item->type() == BookmarkItem::Folder);

    std::unique_ptr<QDialog> dialog;
    switch (item->type()) {
    case BookmarkItem::Url:
        dialog.reset(new BookmarkDialog { *item, m_context.window() });
        dialog->setWindowTitle(tr("Bookmark Property"));
        break;
    case BookmarkItem::Folder:
        dialog.reset(new FolderDialog { *item, m_context.window() });
        dialog->setWindowTitle(tr("Folder Property"));
        break;
    default:
        assert(false && "unsupported bookmark item type.");
        break;
    }

    const auto result = dialog->exec();
    if (result == QDialog::Accepted) {
        bookmarks().changeBookmark(item);
    }
}

Bookmarks &ContextMenu::
bookmarks() const
{
    assert(mApp);
    auto* const bookmarks = mApp->bookmarks();
    assert(bookmarks);

    return *bookmarks;
}

void ContextMenu::
insertBookmark(BookmarkItem &item) const
{
    auto &bookmarks = this->bookmarks();
    auto* const model = bookmarks.model();
    assert(model);

    auto* const contextItem = m_context.currentItem();
    if (contextItem) {
        const auto &index = model->index(contextItem);
        assert(index.isValid());

        bookmarks.insertBookmark(
                  &m_context.parentItem(), index.row() + 1, &item);
    }
    else {
        bookmarks.addBookmark(&m_context.parentItem(), &item);
    }
}

} // namespace bookmark_dash
