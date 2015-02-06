#include "settingdialog.h"

#include "core/styles.h"
#include "action/addstyle.h"
#include "action/openstyle.h"

#include <QtGui/QFileDialog>

namespace stylist {

SettingDialog::
SettingDialog(QWidget &parent)
    : QDialog { &parent }
{
    m_ui.setupUi(this);

    // this will take ownership of actions
    auto* const addStyleAction = new action::AddStyle { nullptr };
    this->addAction(addStyleAction);

    auto* const openStyleAction = new action::OpenStyle;
    this->addAction(openStyleAction);

    // connect with button
    auto *button = m_ui.m_addStyleSheetButton;
    assert(button);
    this->connect(button,         SIGNAL(clicked(bool)),
                  addStyleAction, SIGNAL(triggered(bool)));

    // connect with StylesView
    auto *stylesView = m_ui.m_stylesView;
    assert(stylesView);
    this->connect(stylesView, SIGNAL(openStyle(const Path&)),
                  openStyleAction, SLOT(run(const Path&)));
}

} // namespace stylist
