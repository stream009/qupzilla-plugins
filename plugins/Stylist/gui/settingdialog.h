#ifndef STYLIST_SETTING_DIALOG_H
#define STYLIST_SETTING_DIALOG_H

#include "ui_settingdialog.h"

#include <QtGui/QDialog>

class QItemSelection;
class QWidget;

namespace stylist {

class SettingDialog : public QDialog
{
    Q_OBJECT
public:
    SettingDialog(QWidget &parent);

private Q_SLOTS:
    void addStyle();

private:
    Ui::SettingDialog m_ui;
};

} // namespace stylist

#endif // STYLIST_SETTING_DIALOG_H
