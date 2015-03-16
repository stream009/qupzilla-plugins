#ifndef BOOKMARK_DASH_WINDOW_ADAPTOR_H
#define BOOKMARK_DASH_WINDOW_ADAPTOR_H

#include "iconchangewatcher.h"
#include "menuadaptor.h"
#include "settingsmenu.h"
#include "toolbaradaptor.h"

class BrowserWindow;

namespace bookmark_dash {

class Bar;
class Menu;
class Plugin;
class Settings;

class WindowAdaptor : public QObject
{
    Q_OBJECT
public:
    WindowAdaptor(BrowserWindow&, Plugin&);
    ~WindowAdaptor();

    BrowserWindow &window() const;

private Q_SLOTS:
    void onBookmarkTriggered(BookmarkItem&);

private:
    QMenu &toolsMenu() const;

private:
    BrowserWindow &m_window;
    Plugin &m_plugin;
    MenuAdaptor m_menu;
    ToolBarAdaptor m_toolBar;
    SettingsMenu m_settingsMenu;
    IconChangeWatcher m_iconWatcher;
};

} // namespace bookmark_dash

#endif // BOOKMARK_DASH_WINDOW_ADAPTOR_H
