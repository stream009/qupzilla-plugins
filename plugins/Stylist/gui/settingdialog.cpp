#include "settingdialog.h"

#include "core/styles.h"

#include <QtGui/QDesktopServices>
#include <QtGui/QItemSelection>
#include <QtGui/QItemSelectionModel>
#include <QtGui/QWidget>

namespace stylist {

SettingDialog::
SettingDialog(QWidget &parent)
    : QDialog { &parent },
      m_model { Styles::instance() },
      m_openAction { tr("&Open"), nullptr }
{
    m_ui.setupUi(this);

    auto* const stylesView = m_ui.m_stylesView;
    assert(stylesView);

    stylesView->setModel(&m_model);
    stylesView->setContextMenuPolicy(Qt::ActionsContextMenu);
    stylesView->addAction(&m_openAction);

    auto* const selection = stylesView->selectionModel();
    assert(selection);
    this->connect(selection, SIGNAL(
            selectionChanged(const QItemSelection&, const QItemSelection&)),
                  this,      SLOT(
            slotSelectionChanged(const QItemSelection&, const QItemSelection&))
    );

    this->connect(&m_openAction, SIGNAL(triggered()),
                  this,          SLOT(openEditor()));

    slotSelectionChanged(QItemSelection {}, QItemSelection {});

    assert(m_ui.m_stylesView);
}

void SettingDialog::
openEditor()
{
    qDebug() << __func__;

    auto* const stylesView = m_ui.m_stylesView;
    assert(stylesView);

    const auto current = stylesView->currentIndex();
    assert(current.isValid());

    const auto *model = stylesView->model();
    assert(model);

    const auto data = model->data(current, StylesItemModel::PathRole);
    assert(data.isValid());

    const auto path = data.toString();
    QDesktopServices::openUrl(path);
}

void SettingDialog::
slotSelectionChanged(const QItemSelection&, const QItemSelection&)
{
    qDebug() << __func__;

    auto* const selectionModel = m_ui.m_stylesView->selectionModel();
    assert(selectionModel);

    if (selectionModel->hasSelection()) {
        m_openAction.setEnabled(true);
    }
    else {
        m_openAction.setEnabled(false);
    }
}

} // namespace stylist
