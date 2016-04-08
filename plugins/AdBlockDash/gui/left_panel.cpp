#include "left_panel.hpp"

#include "core/settings.hpp"
#include "filter_list_viewer.hpp"
#include "subscriptions_viewer.hpp"

#include <QtWidgets/QAction>

namespace adblock_dash { namespace setting_dialog {

LeftPanel::
LeftPanel(Settings &settings, SubscriptionsViewer &subscriptionsViewer,
                              CustomFilterSetsViewer &filterSetsViewer,
                              QWidget &parent)
    : Base { &parent }
    , m_tabWidget { new QTabWidget { this } }
    , m_subscriptions {
            new SubscriptionsTab { settings, subscriptionsViewer, *this } }
    , m_customFilterSets {
            new CustomFilterSetsTab { settings, filterSetsViewer, *this } }
    , m_subscriptionsViewer { subscriptionsViewer }
    , m_filterSetsViewer { filterSetsViewer }
{
    assert(m_tabWidget);
    assert(m_subscriptions);
    assert(m_customFilterSets);

    initWidgets();
    layoutWidgets();
    connectWidgets();

    validate();
}

void LeftPanel::initWidgets() {}

void LeftPanel::
layoutWidgets()
{
    m_tabWidget->addTab(m_subscriptions, tr("Filter Subscriptions"));
    m_tabWidget->addTab(m_customFilterSets, tr("Custom filters"));

    auto* const layout = new QVBoxLayout { this };
    assert(layout);
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addWidget(m_tabWidget);
}

void LeftPanel::
connectWidgets()
{
    this->connect(m_tabWidget, SIGNAL(currentChanged(int)),
                  this,          SLOT(onCurrentTabChanged()));
}

void LeftPanel::
onCurrentTabChanged()
{
    m_subscriptionsViewer.clearSelection();
    m_filterSetsViewer.clearSelection();
}

}} // namespace adblock_dash::setting_dialog
