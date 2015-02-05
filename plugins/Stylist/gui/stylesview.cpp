#include "stylesview.h"

#include "common/utility.h"
#include "core/styles.h"

#include <boost/filesystem.hpp>

#include <QtGui/QAction>
#include <QtGui/QDesktopServices>
#include <QtGui/QItemSelection>
#include <QtGui/QItemSelectionModel>

namespace stylist {

StylesView::
StylesView(QWidget* const parent /*= 0*/)
    : QListView { parent },
      m_model { Styles::instance() },
      m_styleActions { nullptr }
{
    this->setModel(&m_model);
    this->setContextMenuPolicy(Qt::ActionsContextMenu);

    // m_styleActions will take ownership of actions
    auto* const openAction = new QAction { tr("&Open"), &m_styleActions };
    this->addAction(openAction);
    this->connect(openAction, SIGNAL(triggered()),
                  this,       SLOT(openEditor()));

    auto* const renameAction = new QAction { tr("&Rename"), &m_styleActions };
    this->addAction(renameAction);
    this->connect(renameAction, SIGNAL(triggered()),
                  this,         SLOT(renameStyle()));

    auto* const selection = this->selectionModel();
    assert(selection);
    this->connect(selection, SIGNAL(
            selectionChanged(const QItemSelection&, const QItemSelection&)),
                  this,      SLOT(
            slotSelectionChanged(const QItemSelection&, const QItemSelection&))
    );

    slotSelectionChanged(QItemSelection {}, QItemSelection {});
}

void StylesView::
openEditor()
{
    //qDebug() << __func__;
    const auto current = this->currentIndex();
    assert(current.isValid());

    const auto* model = this->model();
    assert(current.isValid());

    const auto data = model->data(current, StylesItemModel::PathRole);
    assert(data.isValid());
    assert(strcmp(data.typeName(), "boost::filesystem::path") == 0);

    const auto path = data.value<boost::filesystem::path>();
    const QUrl url { path.c_str() };
    qDebug() << "Opening" << url;
    QDesktopServices::openUrl(url);
}

void StylesView::
renameStyle()
{
    //qDebug() << __func__;
    const auto current = this->currentIndex();
    assert(current.isValid());

    this->edit(current);
}

void StylesView::
slotSelectionChanged(const QItemSelection&, const QItemSelection&)
{
    //qDebug() << __func__;
    auto* const selectionModel = this->selectionModel();
    assert(selectionModel);

    if (selectionModel->hasSelection()) {
        m_styleActions.setEnabled(true);
    }
    else {
        m_styleActions.setEnabled(false);
    }
}

} // namespace stylist
