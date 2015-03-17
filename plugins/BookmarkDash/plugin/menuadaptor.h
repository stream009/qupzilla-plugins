#ifndef MENU_DASH_MENU_ADAPTOR_H
#define MENU_DASH_MENU_ADAPTOR_H

#include <memory>
#include <unordered_map>
#include <vector>

#include <QtCore/QList>
#include <QtCore/QObject>
#include <QtGui/QKeySequence>

class BrowserWindow;
class QAction;
class QMenu;

namespace bookmark_dash {

class RootMenu;
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
    void install(bool);

private:
    void saveOriginal();
    void install();
    void uninstall();

private:
    BrowserWindow &m_window;
    QMenu *m_original = nullptr;
    std::unique_ptr<RootMenu> m_menu;
    std::unique_ptr<MenuButton> m_menuButton;
    std::vector<QAction*> m_installedShortcutActions;
    std::unordered_map<QAction*, QList<QKeySequence>> m_shortcutBackup;
    bool m_installed = false;
};

} // namespace bookmark_dash

#endif // MENU_DASH_MENU_ADAPTOR_H
