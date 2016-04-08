#include "filter_sets_actions.hpp"

#include "common/utility.hpp"
#include "qt_widget_list/abstract_widget_list.hpp"

#include <cassert>

namespace adblock_dash { namespace setting_dialog {

FilterSetsActions::
FilterSetsActions()
{
    setupActions();
}

void FilterSetsActions::
setWidgetList(AbstractWidgetList &list)
{
    assert(!m_list);
    m_list = &list;
}

void FilterSetsActions::
addToWidget(QWidget &widget)
{
    widget.addAction(&m_delete);
    widget.addAction(&m_moveUp);
    widget.addAction(&m_moveDown);
}

static void
setupAction(QAction &action,
            const QString &title, const QKeySequence &shortcut)
{
    action.setText(title);
    action.setShortcut(shortcut);
    action.setShortcutContext(Qt::WidgetShortcut);
}

static void
setupAction(QAction &action,
            const QString &title, const QKeySequence &shortcut,
            QObject &target, const char* const slot)
{
    setupAction(action, title, shortcut);
    action.connect(&action, SIGNAL(triggered()), &target, slot);
}

void FilterSetsActions::
setFilterSetEnabled(const size_t row, const bool enabled)
{
    Q_EMIT enableFilterSet(row, enabled);
}

void FilterSetsActions::
setupActions()
{
    setupAction(m_delete,
            "Delete...", Qt::Key_Delete, *this, SLOT(onDelete()));

    setupAction(m_moveUp,
            "Move up", Qt::CTRL + Qt::Key_Up, *this, SLOT(onMoveUp()));

    setupAction(m_moveDown,
            "Move down", Qt::CTRL + Qt::Key_Down, *this, SLOT(onMoveDown()));
}

void FilterSetsActions::
onDelete()
{
    assert(m_list);

    auto* const model = m_list->model();
    assert(model);
    const auto &selected = m_list->selection();
    assert(selected.isValid());

    const auto &result = QMessageBox::question(
            m_list->window(),
            tr("Delete filter set"),
            tr("Do you want to delete this filter set?"),
            QMessageBox::Ok | QMessageBox::Cancel);
    if (result == QMessageBox::Ok) {
        Q_EMIT deleteFilterSet(::toSizeT(selected.row()));
    }
}

void FilterSetsActions::
onMoveUp()
{
    assert(m_list);

    const auto &selected = m_list->selection();
    assert(selected.isValid());

    const auto row = selected.row();
    if (row == 0) return;

    Q_EMIT moveFilterSet(::toSizeT(row), ::toSizeT(row - 1));
}

void FilterSetsActions::
onMoveDown()
{
    assert(m_list);

    auto* const model = m_list->model();
    assert(model);

    const auto &selected = m_list->selection();
    assert(selected.isValid());

    const auto row = selected.row();
    if (row == model->rowCount() - 1) return;

    Q_EMIT moveFilterSet(::toSizeT(row), ::toSizeT(row + 1));
}

}} // namespace adblock_dash::setting_dialog
