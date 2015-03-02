#ifndef BOOKMARK_DASH_WINDOW_ADAPTOR_H
#define BOOKMARK_DASH_WINDOW_ADAPTOR_H

#include <memory>

class BrowserWindow;
class BookmarksMenu;

namespace bookmark_dash {

class MenuDash;

class WindowAdaptor
{
public:
    WindowAdaptor(BrowserWindow &window);
    ~WindowAdaptor();

private:
    BrowserWindow &m_window;
    BookmarksMenu *m_originalMenu = nullptr;
    std::unique_ptr<MenuDash> m_menu;
};

} // namespace bookmark_dash

#endif // BOOKMARK_DASH_WINDOW_ADAPTOR_H
