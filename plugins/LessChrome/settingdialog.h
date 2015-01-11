#ifndef SETTING_DIALOG_H
#define SETTING_DIALOG_H

#include "ui_settingdialog.h"

class QShowEvent;

namespace lesschrome {

class Settings;

class SettingDialog : public QDialog
{
public:
    SettingDialog(QWidget &parent);

private:
    // @override QDialog
    virtual void accept();
    virtual void showEvent(QShowEvent* const);

private:
    Ui::SettingDialog m_ui;
};

} // namespace lesschrome

#endif // SETTING_DIALOG_H
