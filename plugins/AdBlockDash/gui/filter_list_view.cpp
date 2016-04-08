#include "filter_list_view.hpp"

#include "filter_list_model.hpp"

#include <QtCore/QAbstractItemModel>
#include <QtCore/QDebug> //TODO remove
#include <QtGui/QCursor>
#include <QtWidgets/QMenu>
#include <QtWidgets/QWidget>

namespace adblock_dash { namespace setting_dialog {

FilterListView::
FilterListView(QWidget* const parent)
    : Base { parent }
{
    this->setSelectionMode(QAbstractItemView::ExtendedSelection);
    this->setSelectionBehavior(QAbstractItemView::SelectRows);
    this->setHorizontalScrollMode(QAbstractItemView::ScrollPerPixel);
    this->setShowGrid(false);
    this->setWordWrap(false);

    auto* const hHeader = this->horizontalHeader();
    assert(hHeader);
    hHeader->setDefaultAlignment(Qt::AlignLeft);
    hHeader->setHighlightSections(false);
    hHeader->setStretchLastSection(true);

    auto* const vHeader = this->verticalHeader();
    assert(vHeader);
    vHeader->setDefaultSectionSize(this->fontMetrics().height()+2);
    vHeader->setVisible(false);
}

void FilterListView::
setModel(QAbstractItemModel* const model)
{
    Base::setModel(model);

    if (this->isVisible()) {
        // Unexpected delay happens if widget isn't visible.
        this->resizeColumnToContents(0);
    }

    Q_EMIT modelChanged();
}

void FilterListView::
contextMenuEvent(QContextMenuEvent* const)
{
    const auto &pos = QCursor::pos();

    auto* const viewport = this->viewport();
    assert(viewport);

    const auto &index = this->indexAt(viewport->mapFromGlobal(pos));
    if (!index.isValid()) return;

    auto* const sModel = this->selectionModel();
    assert(sModel);
    if (!sModel->isSelected(index)) return;

    Q_EMIT contextMenuRequested(pos);
}

void FilterListView::
showEvent(QShowEvent* const)
{
    if (this->isVisible()) {
        // Unexpected delay happens if widget isn't visible.
        this->resizeColumnToContents(0);
    }
}

}} // namespace adblock_dash::setting_dialog
