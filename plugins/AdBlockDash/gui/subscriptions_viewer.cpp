#include "subscriptions_viewer.hpp"

#include "core/adblock_dash.hpp"
#include "subscriptions_view.hpp"

namespace adblock_dash { namespace setting_dialog {

SubscriptionsViewer::
SubscriptionsViewer(Settings &settings, QAction &toggleRightPanel)
    : m_settings { settings }
    , m_widget { new SubscriptionsView {
            m_settings.subscriptions(), m_actions, toggleRightPanel } }
{
    assert(m_widget);

    m_actions.setWidgetList(*m_widget);

    this->connect(
        m_widget, SIGNAL(selectionChanged(const QModelIndex&, const QModelIndex&)),
        this,       SLOT(onSelectionChanged(const QModelIndex&, const QModelIndex&)));

    this->connect(
        &m_actions, SIGNAL(deleteFilterSet(const size_t)),
        this,         SLOT(deleteSubscription(const size_t)));
    this->connect(
        &m_actions, SIGNAL(moveFilterSet(const size_t, const size_t)),
        this,         SLOT(moveSubscription(const size_t, const size_t)));
    this->connect(
        &m_actions, SIGNAL(enableFilterSet(const size_t, const bool)),
        this,         SLOT(enableFilterSet(const size_t, const bool)));

    validate();
}

SubscriptionsViewer::
~SubscriptionsViewer()
{
    if (m_widget != nullptr && m_widget->parent() == nullptr) {
        m_widget->deleteLater();
    }
}

Settings::Subscriptions &SubscriptionsViewer::
subscriptions()
{
    return m_settings.subscriptions();
}

void SubscriptionsViewer::
clearSelection()
{
    m_widget->clearSelection();
    Q_EMIT selectionCleared();
}

void SubscriptionsViewer::
deleteSubscription(const size_t row)
{
    namespace bfs = boost::filesystem;

    auto &subscriptions = m_settings.subscriptions();
    const auto &it = subscriptions.begin() + ::toSignedInt(row);
    const auto &path = it->path();
    bfs::exists(path);

    bfs::remove(path);

    subscriptions.erase(it);

    Q_EMIT requestReload();
}

void SubscriptionsViewer::
moveSubscription(const size_t from, const size_t to)
{
    auto &subscriptions = m_settings.subscriptions();
    assert(from < subscriptions.size());
    assert(to < subscriptions.size());
    assert(from != to);

    subscriptions.move(from, to);

    m_widget->select(to);
}

void SubscriptionsViewer::
enableFilterSet(const size_t row, const bool enabled)
{
    auto &subscriptions = m_settings.subscriptions();
    assert(row < subscriptions.size());

    subscriptions[row].setEnabled(enabled);

    Q_EMIT requestReload();
}

void SubscriptionsViewer::
allowNonIntrusiveAds(const bool allow)
{
    m_settings.exceptionRules().setEnabled(allow);

    Q_EMIT requestReload();
}

void SubscriptionsViewer::
openUrl(const QUrl &url)
{
    assert(url.isValid());

    Q_EMIT openUrlRequest(url);
}

void SubscriptionsViewer::
onSelectionChanged(const QModelIndex&, const QModelIndex &after)
{
    if (!after.isValid()) return;

    const auto &subscriptions = m_settings.subscriptions();
    const auto &row = ::toSizeT(after.row());
    const auto &path = subscriptions[row].path();;

    Q_EMIT selected(path, false);
}

}} // adblock_dash::setting_dialog
