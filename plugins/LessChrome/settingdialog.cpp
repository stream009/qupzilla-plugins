#include "settingdialog.h"

#include "lc_settings.h"
#include "plugin.h"

#include <QtCore/QDebug>

namespace lesschrome {

SettingDialog::
SettingDialog(QWidget &parent)
    : QDialog(&parent)
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
    settings.menuBar = m_ui.menuBar->isChecked();

    static const int menuBarKeyTbl[3] = {
        Qt::Key_Shift, Qt::Key_Control, Qt::Key_Alt
    };
    const int idx = m_ui.menuBarKey->currentIndex();
    assert(0 <= idx && idx <= 2);
    settings.menuBarKey = menuBarKeyTbl[idx];

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
    m_ui.menuBar->setChecked(settings.menuBar);

    int idx = -1;
    switch (settings.menuBarKey) {
    case Qt::Key_Shift:
        idx = 0;
        break;
    case Qt::Key_Control:
        idx = 1;
        break;
    case Qt::Key_Alt:
        idx = 2;
        break;
    }
    assert(idx != -1);
    m_ui.menuBarKey->setCurrentIndex(idx);

    m_ui.waitTimer->setValue(settings.waitTimer);
}

} // namespace lesschrome
