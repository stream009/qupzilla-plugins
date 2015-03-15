#ifndef BOOKMARK_DASH_WINDOW_ADAPTOR_H
#define BOOKMARK_DASH_WINDOW_ADAPTOR_H

#include "iconchangewatcher.h"
#include "menuadaptor.h"
#include "settingsmenu.h"
#include "toolbaradaptor.h"

class BrowserWindow;

namespace bookmark_dash {

class Menu;
class Bar;
class Settings;

class WindowAdaptor
{
public:
    WindowAdaptor(BrowserWindow &, Settings &);
    ~WindowAdaptor();

private:
    QMenu &toolsMenu() const;

private:
    BrowserWindow &m_window;
    MenuAdaptor m_menu;
    ToolBarAdaptor m_toolBar;
    SettingsMenu m_settingsMenu;
    IconChangeWatcher m_iconWatcher;
};

} // namespace bookmark_dash

#endif // BOOKMARK_DASH_WINDOW_ADAPTOR_H
