#include "settingsmenu.h"

#include "settings.h"

#include <QtGui/QAction>

namespace bookmark_dash {

SettingsMenu::
SettingsMenu(Settings &settings)
{
    this->setTitle(tr("Bookmark &Dash"));

    m_menuAction.setText(tr("Replace Bookmark &Menu"));
    m_menuAction.setCheckable(true);
    m_menuAction.setChecked(settings.bookmarksMenu().value());
    this->addAction(&m_menuAction);

    m_barAction.setText(tr("Show Bookmark &Bar"));
    m_barAction.setCheckable(true);
    m_barAction.setChecked(settings.bookmarksBar().value());
    this->addAction(&m_barAction);

    this->connect(&settings.bookmarksMenu(), SIGNAL(changed(bool)),
                  &m_menuAction,               SLOT(setChecked(bool)));
    this->connect(&m_menuAction,             SIGNAL(toggled(bool)),
                  &settings.bookmarksMenu(),   SLOT(setValue(bool)));

    this->connect(&settings.bookmarksBar(), SIGNAL(changed(bool)),
                  &m_barAction,               SLOT(setChecked(bool)));
    this->connect(&m_barAction,             SIGNAL(toggled(bool)),
                  &settings.bookmarksBar(),   SLOT(setValue(bool)));
}

} // namespace bookmark_dash
