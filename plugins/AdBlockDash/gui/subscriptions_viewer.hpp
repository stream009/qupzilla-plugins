#ifndef ADBLOCK_DASH_SUBSCRIPTIONS_VIEWER_HPP
#define ADBLOCK_DASH_SUBSCRIPTIONS_VIEWER_HPP

#include "core/settings.hpp"
#include "filter_sets_actions.hpp"
#include "subscriptions_view.hpp"

#include <cassert>

#include <QtCore/QObject>
#include <QtCore/QPointer>

namespace adblock_dash {

class AdBlockDash;

namespace setting_dialog {

class FilterSetsActions;
class SubscriptionsView;

class SubscriptionsViewer : public QObject
{
    Q_OBJECT
public:
    SubscriptionsViewer(Settings&, QAction &toggleRightPanel);
    ~SubscriptionsViewer();

    // accessor
    SubscriptionsView &widget() { return *m_widget; }
    Settings::Subscriptions &subscriptions();

    // modifier
           void clearSelection();
    Q_SLOT void deleteSubscription(const size_t);
    Q_SLOT void moveSubscription(const size_t from, const size_t to);
    Q_SLOT void enableFilterSet(const size_t row, const bool enabled);
           void allowNonIntrusiveAds(const bool);

    // command
    void openUrl(const QUrl&);

Q_SIGNALS:
    void openUrlRequest(const QUrl&);
    void requestReload();
    void selected(const Path&, const bool);
    void selectionCleared();

private Q_SLOTS:
    void onSelectionChanged(const QModelIndex&, const QModelIndex &);

private:
    void validate() const
    {
        assert(m_widget);
    }

private:
    Settings &m_settings;
    FilterSetsActions m_actions;
    QPointer<SubscriptionsView> m_widget;
};

}} // adblock_dash::setting_dialog

#endif // ADBLOCK_DASH_SUBSCRIPTIONS_VIEWER_HPP
