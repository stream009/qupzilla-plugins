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
    void onBookmarkRemoveRequested();

private:
    QMenu &toolsMenu() const;

    void validate()
    {
        assert(m_remover);
    }

private:
    BrowserWindow &m_window;
    Plugin &m_plugin;
    MenuAdaptor m_menu;
    ToolBarAdaptor m_toolBar;
    SettingsMenu m_settingsMenu;
    IconChangeWatcher m_iconWatcher;
    QAction* m_remover = nullptr;
};

} // namespace bookmark_dash

#endif // BOOKMARK_DASH_WINDOW_ADAPTOR_H
