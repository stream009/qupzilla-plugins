#ifndef BOOKMARK_DASH_ROOT_MENU_H
#define BOOKMARK_DASH_ROOT_MENU_H

#include "menu.h"

class BrowserWindow;
class QWidget;

namespace bookmark_dash {

class RootMenu : public Menu
{
    Q_OBJECT
    using Base = Menu;
public:
    RootMenu(BrowserWindow&, QWidget* const parent);

private Q_SLOTS:
    void onBookmarkThisPage();
    void onBookmarkAllTabs();
    void onOrganizeBookmarks();

private:
    BrowserWindow &m_window;
};

} // namespace bookmark_dash

#endif // BOOKMARK_DASH_ROOT_MENU_H
