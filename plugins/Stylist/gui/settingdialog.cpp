#include "settingdialog.h"

#include "../plugin.h"

#include <QtGui/QWidget>

namespace stylist {

SettingDialog::
SettingDialog(QWidget &parent)
    : QDialog { &parent },
      m_model { Plugin::styles() }
{
    m_ui.setupUi(this);

    m_ui.m_stylesView->setModel(&m_model);
}

} // namespace stylist
