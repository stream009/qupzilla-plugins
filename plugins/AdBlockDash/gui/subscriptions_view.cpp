#include "subscriptions_view.hpp"

#include "filter_set_widget.hpp"
#include "filter_sets_actions.hpp"
#include "subscription_widget.hpp"
#include "subscriptions_model.hpp"

#include <QtWidgets/QWidget>

namespace adblock_dash { namespace setting_dialog {

SubscriptionsView::
SubscriptionsView(Settings::Subscriptions &subscriptions,
                  FilterSetsActions &actions,
                  QAction &toggleRightPanel,
                  QWidget* const parent/*= nullptr*/)
    : Base { parent }
    , m_model { subscriptions }
    , m_actions { actions }
    , m_toggleRightPanel { toggleRightPanel }
{
    this->setModel(&m_model);
}

QWidget &SubscriptionsView::
createItemWidget(const QModelIndex &index)
{
    auto* const content = new SubscriptionWidget { m_model, index, *this };
    assert(content);

    auto* const item = new FilterSetWidget {
        *this,
        index,
        *content, // ownership will be transfered
        m_actions,
        m_toggleRightPanel
    };
    assert(item);

    this->connect(item,    SIGNAL(editTitle()),
                  content,   SLOT(editTitle()));
    return *item;
}

}} // namespace adblock_dash::setting_dialog
