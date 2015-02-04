#include "settingdialog.h"

#include "common/utility.h"
#include "core/styles.h"

#include <boost/filesystem.hpp>

#include <QtGui/QDesktopServices>
#include <QtGui/QItemSelection>
#include <QtGui/QItemSelectionModel>
#include <QtGui/QWidget>

namespace stylist {

SettingDialog::
SettingDialog(QWidget &parent)
    : QDialog { &parent },
      m_model { Styles::instance() },
      m_styleActions { nullptr }
{
    m_ui.setupUi(this);
    m_styleActions.setExclusive(false);

    auto* const stylesView = m_ui.m_stylesView;
    assert(stylesView);

    stylesView->setModel(&m_model);
    stylesView->setContextMenuPolicy(Qt::ActionsContextMenu);

    // m_styleActions will take ownership of actions
    auto* const openAction = new QAction { tr("&Open"), &m_styleActions };
    stylesView->addAction(openAction);
    this->connect(openAction, SIGNAL(triggered()),
                  this,       SLOT(openEditor()));

    auto* const renameAction = new QAction { tr("&Rename"), &m_styleActions };
    stylesView->addAction(renameAction);
    this->connect(renameAction, SIGNAL(triggered()),
                  this,         SLOT(renameStyle()));

    auto* const selection = stylesView->selectionModel();
    assert(selection);
    this->connect(selection, SIGNAL(
            selectionChanged(const QItemSelection&, const QItemSelection&)),
                  this,      SLOT(
            slotSelectionChanged(const QItemSelection&, const QItemSelection&))
    );

    slotSelectionChanged(QItemSelection {}, QItemSelection {});

    assert(m_ui.m_stylesView);
}

void SettingDialog::
openEditor()
{
    //qDebug() << __func__;
    auto* const stylesView = m_ui.m_stylesView;
    assert(stylesView);

    const auto current = stylesView->currentIndex();
    assert(current.isValid());

    const auto *model = stylesView->model();
    assert(model);

    const auto data = model->data(current, StylesItemModel::PathRole);
    assert(data.isValid());
    assert(strcmp(data.typeName(), "boost::filesystem::path") == 0);

    const auto path = data.value<boost::filesystem::path>();
    const QUrl url { path.c_str() };
    qDebug() << "Opening" << url;
    QDesktopServices::openUrl(url);
}

void SettingDialog::
renameStyle()
{
    //qDebug() << __func__;
    auto* const stylesView = m_ui.m_stylesView;
    assert(stylesView);

    const auto current = stylesView->currentIndex();
    assert(current.isValid());

    stylesView->edit(current);
}

void SettingDialog::
slotSelectionChanged(const QItemSelection&, const QItemSelection&)
{
    //qDebug() << __func__;
    auto* const selectionModel = m_ui.m_stylesView->selectionModel();
    assert(selectionModel);

    if (selectionModel->hasSelection()) {
        m_styleActions.setEnabled(true);
    }
    else {
        m_styleActions.setEnabled(false);
    }
}

} // namespace stylist
