#include "custom_filter_sets_viewer.hpp"

#include "core/adblock_dash.hpp"

#include <fstream>

namespace adblock_dash { namespace setting_dialog {

CustomFilterSetsViewer::
CustomFilterSetsViewer(Settings::FilterSets &filterSets,
                       QAction &toggleRightPanel)
    : m_filterSets { filterSets }
    , m_widget { new CustomFilterSetsView { filterSets, m_actions, toggleRightPanel } }
{
    assert(m_widget);

    m_actions.setWidgetList(*m_widget);

    this->connect(
        m_widget, SIGNAL(selectionChanged(const QModelIndex&, const QModelIndex&)),
        this,       SLOT(onSelectionChanged(const QModelIndex&, const QModelIndex&)));

    this->connect(
        &m_actions, SIGNAL(deleteFilterSet(const size_t)),
        this,         SLOT(deleteFilterSet(const size_t)));
    this->connect(
        &m_actions, SIGNAL(moveFilterSet(const size_t, const size_t)),
        this,         SLOT(moveFilterSet(const size_t, const size_t)));
    this->connect(
        &m_actions, SIGNAL(enableFilterSet(const size_t, const bool)),
        this,         SLOT(enableFilterSet(const size_t, const bool)));

    validate();
}

CustomFilterSetsViewer::
~CustomFilterSetsViewer()
{
    if (m_widget != nullptr && m_widget->parent() == nullptr) {
        m_widget->deleteLater();
    }
}

void CustomFilterSetsViewer::
clearSelection()
{
    m_widget->clearSelection();
    Q_EMIT selectionCleared();
}

void CustomFilterSetsViewer::
newFilterSet(const QString &name, const Path &path)
{
    namespace bfs = boost::filesystem;

    m_filterSets.emplace_back(name, path);

    if (!bfs::exists(path)) {
        std::ofstream ofs {
                path.c_str(), std::ios_base::out | std::ios_base::trunc };
        ofs << "[Adblock Plus 2.0]\n";
    }

    Q_EMIT requestReload();
}

void CustomFilterSetsViewer::
deleteFilterSet(const size_t row)
{
    namespace bfs = boost::filesystem;

    const auto &it = m_filterSets.begin() + ::toSignedInt(row);
    const auto &path = it->path();
    bfs::exists(path);

    Q_EMIT requestReload();

    bfs::remove(path);

    m_filterSets.erase(it);
}

void CustomFilterSetsViewer::
moveFilterSet(const size_t from, const size_t to)
{
    assert(from < m_filterSets.size());
    assert(to < m_filterSets.size());
    assert(from != to);

    m_filterSets.move(from, to);

    m_widget->select(to);
}

void CustomFilterSetsViewer::
enableFilterSet(const size_t row, const bool enabled)
{
    assert(row < m_filterSets.size());

    m_filterSets[row].setEnabled(enabled);

    Q_EMIT requestReload();
}

void CustomFilterSetsViewer::
onSelectionChanged(const QModelIndex&, const QModelIndex &after)
{
    if (!after.isValid()) return;

    const auto &row = ::toSizeT(after.row());
    const auto &path = m_filterSets[row].path();;

    Q_EMIT selected(path, true);
}

}} // namespace adblock_dash::setting_dialog
