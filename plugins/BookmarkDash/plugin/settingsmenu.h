#ifndef MENU_DASH_SETTINGS_MENU_H
#define MENU_DASH_SETTINGS_MENU_H

#include <QtGui/QMenu>

namespace bookmark_dash {

class Settings;

class SettingsMenu : public QMenu
{
public:
    SettingsMenu(Settings &);

private:
    QAction m_menuAction { nullptr };
    QAction m_barAction { nullptr };
};

} // namespace bookmark_dash

#endif // MENU_DASH_SETTINGS_MENU_H
