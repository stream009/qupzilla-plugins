#include "filter_list_model.hpp"

#include <QtCore/QDebug>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QTableView>
#include <QtWidgets/QVBoxLayout>

int main(int argc, char *argv[])
{
    QApplication app { argc, argv };

    using namespace adblock_dash;

    QDialog dialog;
    dialog.resize(640, 480);
    auto* const layout = new QVBoxLayout { &dialog };
    assert(layout);
    auto* const view = new QTableView { &dialog };
    assert(view);

    auto* const hHeader = view->horizontalHeader();
    assert(hHeader);
    hHeader->setStretchLastSection(true);
    hHeader->setHighlightSections(false);
    hHeader->setDefaultAlignment(Qt::AlignLeft);

    auto* const vHeader = view->verticalHeader();
    assert(vHeader);
    vHeader->setDefaultSectionSize(view->fontMetrics().height());
    vHeader->hide();

    layout->addWidget(view);

    view->setShowGrid(false);
    view->setWordWrap(false);
    view->setSelectionBehavior(QTableView::SelectRows);
    view->setSelectionMode(QTableView::SingleSelection);

    setting_dialog::FilterListModel model { "data/easylist.txt" };
    view->setModel(&model);

    return dialog.exec();
}
