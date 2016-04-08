#include "settingdialog.h"

#include "ui_settingdialog.h"
#include "settingactions.h"
#include "action/addstyle.h"
#include "action/openstyle.h"
#include "action/renamestyle.h"
#include "action/removestyle.h"

#include "core/styles.h"

#include <QtWidgets/QFileDialog>

namespace stylist {

SettingDialog::
SettingDialog(Styles &styles, QWidget &parent)
    : QDialog { &parent },
      m_ui { new Ui::SettingDialog },
      m_actions { new SettingActions { styles } }
{
    m_ui->setupUi(this);
    m_ui->m_stylesView->setActions(*m_actions);
    m_ui->m_stylesView->setStyles(styles);

    // connect with button
    auto *button = m_ui->m_addStyleSheetButton;
    assert(button);
    auto &addStyleAction = m_actions->addStyle();
    this->connect(button,          SIGNAL(clicked(bool)),
                  &addStyleAction, SIGNAL(triggered(bool)));
}

SettingDialog::~SettingDialog() = default;

} // namespace stylist
