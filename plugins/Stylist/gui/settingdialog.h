#ifndef STYLIST_SETTING_DIALOG_H
#define STYLIST_SETTING_DIALOG_H

#include "ui_settingdialog.h"
#include "stylesitemmodel.h"

#include <QtGui/QAction>
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
    void openEditor();
    void renameStyle();
    void slotSelectionChanged(const QItemSelection&, const QItemSelection &);

private:
    Ui::SettingDialog m_ui;
    StylesItemModel m_model;
    QActionGroup m_styleActions;
};

} // namespace stylist

#endif // STYLIST_SETTING_DIALOG_H
