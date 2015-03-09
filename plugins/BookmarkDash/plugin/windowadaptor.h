#ifndef BOOKMARK_DASH_WINDOW_ADAPTOR_H
#define BOOKMARK_DASH_WINDOW_ADAPTOR_H

#include <memory>

class BookmarksMenu;
class BookmarksToolbar;
class BrowserWindow;

namespace bookmark_dash {

class Menu;
class Bar;

class WindowAdaptor : public QObject
{
    Q_OBJECT
public:
    WindowAdaptor(BrowserWindow &window);
    ~WindowAdaptor();

private:
    void saveOriginalMenu();
    void createMenu();
    void setModelToMenu();
    void installMenu();

private Q_SLOTS:
    void bookmarkPage();
    void bookmarkAllTabs();
    void showBookmarksManager();

private:
    BrowserWindow &m_window;
    BookmarksMenu *m_originalMenu = nullptr;
    BookmarksToolbar *m_originalBar = nullptr;
    std::unique_ptr<Menu> m_menu; // unique_ptr for cut dependency
    std::unique_ptr<Bar> m_bar;
};

} // namespace bookmark_dash

#endif // BOOKMARK_DASH_WINDOW_ADAPTOR_H
