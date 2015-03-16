#ifndef MENU_DASH_MENU_ADAPTOR_H
#define MENU_DASH_MENU_ADAPTOR_H

#include <memory>
#include <unordered_map>

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtGui/QKeySequence>

class BrowserWindow;
class QAction;
class QMenu;

namespace bookmark_dash {

class Menu;
class MenuButton;
class Settings;
class WindowAdaptor;

class MenuAdaptor : public QObject
{
    Q_OBJECT
public:
    MenuAdaptor(WindowAdaptor &, Settings &);
    ~MenuAdaptor() override;

private Q_SLOTS:
    void bookmarkPage();
    void bookmarkAllTabs();
    void showBookmarksManager();
    void install(bool);

private:
    void saveOriginal();
    void createTopMenu();
    void install();
    void uninstall();

private:
    BrowserWindow &m_window;
    QMenu *m_original = nullptr;
    std::unique_ptr<Menu> m_menu;
    std::unique_ptr<MenuButton> m_menuButton;
    QAction *m_bookmarkThisPage = nullptr;
    QAction *m_organizeBookmarks = nullptr;
    std::unordered_map<QAction*, QList<QKeySequence>> m_shortcutBackup;
    bool m_installed = false;
};

} // namespace bookmark_dash

#endif // MENU_DASH_MENU_ADAPTOR_H
