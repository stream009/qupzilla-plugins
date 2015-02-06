#include "stylesview.h"

#include "common/utility.h"
#include "core/styles.h"

#include <QtGui/QAction>
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

    createActions();

    connectWithModel();
    connectWithSelectionModel();

    slotSelectionChanged(QItemSelection {}, QItemSelection {});
}

void StylesView::
slotOpenStyle()
{
    //qDebug() << __func__;
    const auto current = this->currentIndex();
    assert(current.isValid());
    Q_EMIT openStyle(current);
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

void StylesView::
createActions()
{
    // m_styleActions will take ownership of actions
    auto* const openAction = new QAction { tr("&Open"), &m_styleActions };
    this->addAction(openAction);
    this->connect(openAction, SIGNAL(triggered()),
                  this,       SLOT(slotOpenSlot()));

    auto* const renameAction = new QAction { tr("&Rename"), &m_styleActions };
    this->addAction(renameAction);
    this->connect(renameAction, SIGNAL(triggered()),
                  this,         SLOT(renameStyle()));
}

void StylesView::
connectWithModel()
{
    auto* const selection = this->selectionModel();
    assert(selection);
    this->connect(selection, SIGNAL(
            selectionChanged(const QItemSelection&, const QItemSelection&)),
                  this,      SLOT(
            slotSelectionChanged(const QItemSelection&, const QItemSelection&))
    );
}

void StylesView::
connectWithSelectionModel()
{
    this->connect(this,          SIGNAL(openStyle(const QModelIndex&)),
                  this->model(), SLOT(slotOpenStyle(const QModelIndex&)));
    this->connect(this->model(), SIGNAL(openStyle(const Path&)),
                  this,          SIGNAL(openStyle(const Path&)));
}

} // namespace stylist
