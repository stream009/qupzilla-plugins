#ifndef BOOKMARK_DASH_CONTEXT_MENU_H
#define BOOKMARK_DASH_CONTEXT_MENU_H

class BookmarksModel;
class BrowserWindow;
class QModelIndex;

namespace bookmark_dash {

class MenuContext
{
public:
    virtual BrowserWindow &window() const = 0;
    virtual BookmarkItem *currentItem() const = 0;
    virtual BookmarkItem &parentItem() const = 0;
    virtual void openBookmark(BookmarkItem&) = 0;
    virtual void openBookmarkInNewTab(BookmarkItem&) = 0;
    virtual void openBookmarkInNewWindow(BookmarkItem&) = 0;
    virtual void openFolderInTabs(BookmarkItem&) = 0;
};

} // namespace bookmark_dash

#include <QtGui/QAction>
#include <QtGui/QActionGroup>
#include <QtGui/QMenu>

class QWidget;

namespace bookmark_dash {

class ContextMenu : public QMenu
{
    Q_OBJECT
    using Base = QMenu;
public:
    ContextMenu(MenuContext &context, QWidget* const parent);

private Q_SLOTS:
    void onAboutToShow();

    void onOpen();
    void onOpenInNewTab();
    void onOpenInNewWindow();
    void onOpenAllInTabs();

    void onNewBookmark();
    void onNewFolder();
    void onNewSeparator();

    void onCut();
    void onPaste();
    void onDelete();

    void onProperty();

private:
    Bookmarks &bookmarks() const;
    void insertBookmark(BookmarkItem&) const;

private:
    MenuContext &m_context;
    QActionGroup m_urlActions;
    QActionGroup m_folderActions;
    QAction m_propertyAction { nullptr };
};

} // namespace bookmark_dash

#endif // BOOKMARK_DASH_CONTEXT_MENU_H
