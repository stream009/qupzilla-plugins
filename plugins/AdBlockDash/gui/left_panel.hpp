#ifndef ADBLOCK_DASH_LEFT_PANEL_HPP
#define ADBLOCK_DASH_LEFT_PANEL_HPP

#include "subscriptions_tab.hpp"
#include "custom_filter_sets_tab.hpp"

#include <cassert>

#include <QtWidgets/QTabWidget>
#include <QtWidgets/QWidget>

class QAction;

namespace adblock_dash {

class Settings;

namespace setting_dialog {

class FilterListViewer;
class SubscriptionsViewer;

class LeftPanel : public QWidget
{
    Q_OBJECT

    using Base = QWidget;
public:
    LeftPanel(Settings&, SubscriptionsViewer&,
                         CustomFilterSetsViewer&,
                         QWidget &parent);
private:
    void initWidgets();
    void layoutWidgets();
    void connectWidgets();

private Q_SLOTS:
    void onCurrentTabChanged();

private:
    void validate() const
    {
        assert(m_tabWidget && m_tabWidget->parent() == this);
        assert(m_subscriptions && m_subscriptions->parent() != nullptr);
        assert(m_customFilterSets && m_customFilterSets->parent() != nullptr);
    }

private:
    QTabWidget          *m_tabWidget;
    SubscriptionsTab    *m_subscriptions;
    CustomFilterSetsTab *m_customFilterSets;

    SubscriptionsViewer &m_subscriptionsViewer;
    CustomFilterSetsViewer &m_filterSetsViewer;
};

}} // namespace adblock_dash::setting_dialog

#endif // ADBLOCK_DASH_LEFT_PANEL_HPP
