#include "settingdialog.h"

#include "core/styles.h"

#include <QtGui/QFileDialog>

namespace stylist {

SettingDialog::
SettingDialog(QWidget &parent)
    : QDialog { &parent }
{
    m_ui.setupUi(this);

    auto *button = m_ui.m_addStyleSheetButton;
    assert(button);
    this->connect(button, SIGNAL(clicked(bool)),
                  this,   SLOT(addStyle()));
}

void SettingDialog::
addStyle()
{
    const auto &filename = QFileDialog::getOpenFileName(
        this,
        tr("Select style sheet to add"),
        "",
        tr("Style Sheet (*.css)")
    );
    if (filename.isNull()) return;

    qDebug() << __func__ << filename;
    auto &styles = Styles::instance();
    styles.import(filename.toUtf8().constData());
}

} // namespace stylist
