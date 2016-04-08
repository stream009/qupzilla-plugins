#ifndef STYLIST_SETTING_DIALOG_H
#define STYLIST_SETTING_DIALOG_H

#include <memory>

#include <QtWidgets/QDialog>

class QWidget;

namespace Ui {
class SettingDialog;
} // namespace Ui

namespace stylist {

class SettingActions;
class Styles;

class SettingDialog : public QDialog
{
public:
    SettingDialog(Styles &, QWidget &parent);
    ~SettingDialog();

private:
    // Since this class is the gateway to rest of the dialog
    // related classes, we minimize dependency.
    std::unique_ptr<Ui::SettingDialog> m_ui;
    std::unique_ptr<SettingActions> m_actions;
};

} // namespace stylist

#endif // STYLIST_SETTING_DIALOG_H
