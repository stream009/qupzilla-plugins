#include "filter_list_actions.hpp"

#include "common/utility.hpp"
#include "filter_list_model.hpp"
#include "filter_list_view.hpp"

#include <boost/algorithm/cxx11/all_of.hpp>
#include <boost/range/adaptor/reversed.hpp>

#include <QtCore/QItemSelection>
#include <QtCore/QPoint>
#include <QtCore/QString>
#include <QtGui/QClipboard>
#include <QtWidgets/QApplication>

static void //TODO duplication
setupAction(QAction &action,
            const QString &title, const QKeySequence &shortcut = {})
{
    action.setText(title);
    action.setShortcut(shortcut);
    action.setShortcutContext(Qt::WidgetShortcut);
}

static void //TODO duplication
setupAction(QAction &action,
            const QString &title, const QKeySequence &shortcut,
            QObject &target, const char* const slot)
{
    setupAction(action, title, shortcut);
    action.connect(&action, SIGNAL(triggered()), &target, slot);
}

static QModelIndex
selectedRow(QItemSelectionModel &sModel)
{
    if (!sModel.hasSelection()) return {};

    const auto &selected = sModel.selectedRows();
    assert(selected.size() == 1);

    return selected.front();
}

static QClipboard &
clipboard()
{
    assert(qApp);
    auto* const clipboard = qApp->clipboard();
    assert(clipboard);

    return *clipboard;
}

namespace adblock_dash { namespace setting_dialog {

FilterListActions::
FilterListActions(FilterListView &view)
    : m_listView { view }
{
    setupActions();
    setupMenu();

    installActions();

    this->connect(&view, SIGNAL(modelChanged()),
                  this,    SLOT(onModelChanged()));
    this->connect(&view, SIGNAL(contextMenuRequested(const QPoint&)),
                  this,    SLOT(onContextMenuRequested(const QPoint&)));
    assert(qApp);
    auto* const clipboard = qApp->clipboard();
    assert(clipboard);
    this->connect(clipboard, SIGNAL(dataChanged()),
                  this,        SLOT(onClipboardChanged()));
    onClipboardChanged();
}

bool FilterListActions::
search(const QString &keyword,
       const Qt::CaseSensitivity caseSensitivity/*= Qt::CaseInsensitive*/,
       const int step/* = +1*/,
       const int startOffset/* = 0*/)
{
    // Because QAbstractItemModel::match doesn't support backward
    // search, we have to write it.
    assert(step != 0);

    auto* const model = m_listView.model();
    if (!model) return false;

    const auto &currentIndex = m_listView.currentIndex();
    const auto &startRow =
            currentIndex.isValid() ? currentIndex.row() + startOffset : 0;

    QModelIndex match;
    auto row = startRow;
    bool wrapped = false;
    const auto &rowCount = model->rowCount();

    while (true) {
        const auto &index = model->index(row, 1);
        const auto &filter = index.data(Qt::DisplayRole).toString();

        if (filter.contains(keyword, caseSensitivity)) {
            match = index;
            break;
        }

        row += step;

        if (step > 0) {
            if (!wrapped && row >= rowCount) {
                row -= rowCount;
                wrapped = true;
            }
            else if (wrapped && row >= startRow) {
                break;
            }
        }
        else if (step < 0) {
            if (!wrapped && row < 0) {
                row += rowCount;
                wrapped = true;
            }
            else if (wrapped && row <= startRow) {
                break;
            }
        }
    }

    if (match.isValid()) {
        // QAbstractItemView::setCurrentIndex rejects disabled indices.
        // So we do that through the selection model instead.
        auto* const sModel = m_listView.selectionModel();
        assert(sModel);
        sModel->setCurrentIndex(match,
            QItemSelectionModel::SelectCurrent | QItemSelectionModel::Rows);

        return true;
    }
    else {
        return false;
    }
}

void FilterListActions::
add()
{
    const auto &index = newRow();
    m_listView.edit(index);
}

void FilterListActions::
setupActions()
{
    setupAction(m_edit  , "Edit"  , Qt::Key_F2,
                          *this, SLOT(onEdit()));
    setupAction(m_cut   , "Cut"   , Qt::CTRL + Qt::Key_X,
                          *this, SLOT(onCut()));
    setupAction(m_copy  , "Copy"  , Qt::CTRL + Qt::Key_C,
                          *this, SLOT(onCopy()));
    setupAction(m_paste , "Paste" , Qt::CTRL + Qt::Key_V,
                          *this, SLOT(onPaste()));
    setupAction(m_delete, "Delete", Qt::Key_Delete,
                          *this, SLOT(onDelete()));

    setupAction(m_selectAll, "Select All", Qt::CTRL + Qt::Key_A,
                             m_listView, SLOT(selectAll()));

    setupAction(m_resetHitStatus, "Reset Hit Statistics");

    setupAction(m_view, "View");
}

void FilterListActions::
setupMenu()
{
    m_menu.addAction(&m_edit);
    m_menu.addAction(&m_cut);
    m_menu.addAction(&m_copy);
    m_menu.addAction(&m_paste);
    m_menu.addAction(&m_delete);
    m_menu.addSeparator();
    m_menu.addAction(&m_selectAll);
    m_menu.addAction(&m_resetHitStatus);
    m_menu.addSeparator();
    m_menu.addAction(&m_view);
}

void FilterListActions::
installActions()
{
    m_listView.addAction(&m_edit);
    m_listView.addAction(&m_cut);
    m_listView.addAction(&m_copy);
    m_listView.addAction(&m_paste);
    m_listView.addAction(&m_delete);
    m_listView.addAction(&m_selectAll);
}

void FilterListActions::
onClipboardChanged()
{
    m_paste.setEnabled(isEditable() && !clipboard().text().isEmpty());
}

void FilterListActions::
onEdit()
{
    assert(isEditable());

    const auto &current = m_listView.currentIndex();
    assert(current.isValid());

    m_listView.edit(current);
}

void FilterListActions::
onCut()
{
    assert(isEditable());

    onCopy();
    onDelete();
}

void FilterListActions::
onCopy()
{
    auto* const sModel = m_listView.selectionModel();
    assert(sModel);
    auto* const model = m_listView.model();
    assert(model);

    QStringList filters;
    for (const auto &selected: sModel->selectedRows()) {
        assert(selected.isValid());

        const auto &index = model->index(
                selected.row(),
                static_cast<int>(FilterListModel::Column::Name));
        const auto &data = model->data(index);
        assert(data.isValid());

        filters.push_back(data.toString());
    }

    clipboard().setText(filters.join("\n"));
}

void FilterListActions::
onPaste()
{
    assert(isEditable());

    auto &&text = clipboard().text();
    if (text.isNull()) return;

    auto* const model = m_listView.model();
    assert(model);

    if (text.endsWith("\n")) {
        text.chop(1);
    }
    const auto &lines = text.split("\n");
    for (const auto &line: boost::adaptors::reverse(lines)) {
        const auto &index = newRow();
        assert(index.isValid());

        model->setData(index, line);
    }
}

void FilterListActions::
onDelete()
{
    assert(isEditable());

    auto* const sModel = m_listView.selectionModel();
    assert(sModel);

    auto* const model = m_listView.model();
    assert(model);

    for (const auto &selected: sModel->selectedRows()) {
        assert(selected.isValid());

        model->removeRow(selected.row(), selected.parent());
    }
}

void FilterListActions::
onModelChanged()
{
    auto* const sModel = m_listView.selectionModel();
    assert(sModel);
    QObject::connect(
        sModel, SIGNAL(selectionChanged(
                            const QItemSelection&, const QItemSelection&)),
        this,     SLOT(onSelectionChanged(
                            const QItemSelection&, const QItemSelection&))
    );

    onSelectionChanged(sModel->selection(), {});
    onClipboardChanged();
}

void FilterListActions::
onSelectionChanged(const QItemSelection &current, const QItemSelection&)
{
    auto* const model = m_listView.model();
    if (model == nullptr) {
        m_edit.setEnabled(false);
        m_cut.setEnabled(false);
        m_copy.setEnabled(false);
        m_delete.setEnabled(false);
    }
    else {
        const auto &selected = !current.isEmpty();
        const auto &editable = isEditable();

        m_edit.setEnabled(selected && editable);
        m_cut.setEnabled(selected && editable);
        m_copy.setEnabled(selected);
        m_delete.setEnabled(selected && editable);
    }
}

void FilterListActions::
onContextMenuRequested(const QPoint &pos)
{
    m_menu.popup(pos);
}

bool FilterListActions::
isEditable()
{
    auto* const model = dynamic_cast<FilterListModel*>(m_listView.model());
    if (!model) return false;

    return model->isEditable();
}

QModelIndex FilterListActions::
newRow()
{
    assert(isEditable());
    auto* const model = m_listView.model();
    assert(model);

    auto* const sModel = m_listView.selectionModel();
    assert(sModel);
    const auto &selectedIdx = selectedRow(*sModel);

    const auto &row = selectedIdx.isValid() ?
                             selectedIdx.row() + 1 : model->rowCount();

    const auto &rc = model->insertRow(row, selectedIdx.parent());
    assert(rc); (void)rc;

    const auto &newIndex = model->index(
              row, static_cast<int>(FilterListModel::Column::Name));
    m_listView.scrollTo(newIndex);

    return newIndex;
}

}} // namespace adblock_dash::setting_dialog
