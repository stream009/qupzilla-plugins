#include "settingdialog.h"

#include "lc_settings.h"
#include "plugin.h"

#include <QtCore/QDebug>

namespace lesschrome {

SettingDialog::
SettingDialog(QWidget* const parent)
    : QDialog(parent)
{
    m_ui.setupUi(this);
}

void SettingDialog::
accept()
{
    Settings &settings = Plugin::settings();

    settings.navigationBar = m_ui.navigationBar->isChecked();
    settings.bookmarksBar = m_ui.bookmarksBar->isChecked();
    settings.statusBar = m_ui.statusBar->isChecked();
    settings.waitTimer = m_ui.waitTimer->value();

    QDialog::accept();
}

void SettingDialog::
showEvent(QShowEvent* const)
{
    Settings &settings = Plugin::settings();

    m_ui.navigationBar->setChecked(settings.navigationBar);
    m_ui.bookmarksBar->setChecked(settings.bookmarksBar);
    m_ui.statusBar->setChecked(settings.statusBar);
    m_ui.waitTimer->setValue(settings.waitTimer);
}

} // namespace lesschrome
