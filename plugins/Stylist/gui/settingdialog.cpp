#include "settingdialog.h"

#include <QtGui/QWidget>

namespace stylist {

SettingDialog::
SettingDialog(QWidget &parent)
    : QDialog { &parent }
{
    m_ui.setupUi(this);
}

} // namespace stylist
