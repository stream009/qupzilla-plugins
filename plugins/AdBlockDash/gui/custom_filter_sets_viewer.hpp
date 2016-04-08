#ifndef ADBLOCK_DASH_CUSTOM_FILTER_SETS_VIEWER_HPP
#define ADBLOCK_DASH_CUSTOM_FILTER_SETS_VIEWER_HPP

#include "common/path.hpp"
#include "core/settings.hpp"
#include "custom_filter_sets_view.hpp"
#include "filter_sets_actions.hpp"

#include <cassert>

#include <QtCore/QObject>
#include <QtCore/QPointer>

class QAction;
class QModelIndex;

namespace adblock_dash {

class AdBlockDash;

namespace setting_dialog {

class CustomFilterSetsView;

class CustomFilterSetsViewer : public QObject
{
    Q_OBJECT
public:
    CustomFilterSetsViewer(Settings::FilterSets&,
                           QAction &toggleRightPanel);
    ~CustomFilterSetsViewer();

    // accessor
    CustomFilterSetsView &widget() { return *m_widget; }
    Settings::FilterSets &filterSets() { return m_filterSets; }

    // modifier
    void clearSelection();
    void newFilterSet(const QString&, const Path&);

public Q_SLOTS:
    void deleteFilterSet(const size_t);
    void moveFilterSet(const size_t from, const size_t to);
    void enableFilterSet(const size_t row, const bool enabled);

Q_SIGNALS:
    void selected(const Path&, const bool);
    void selectionCleared();
    void requestReload();

private Q_SLOTS:
    void onSelectionChanged(const QModelIndex&, const QModelIndex &);

private:
    void validate() const
    {
        assert(m_widget);
    }

private:
    Settings::FilterSets &m_filterSets;
    FilterSetsActions m_actions;
    QPointer<CustomFilterSetsView> m_widget;
};

}} // namespace adblock_dash::setting_dialog

#endif // ADBLOCK_DASH_CUSTOM_FILTER_SETS_VIEWER_HPP
