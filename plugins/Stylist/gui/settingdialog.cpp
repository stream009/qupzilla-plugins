#include "settingdialog.h"

namespace stylist {

SettingDialog::
SettingDialog(QWidget &parent)
    : QDialog { &parent }
{
    m_ui.setupUi(this);
}

} // namespace stylist
