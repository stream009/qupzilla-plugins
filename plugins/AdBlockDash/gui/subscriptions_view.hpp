#ifndef ADBLOCK_DASH_SUBSCRIPTIONS_VIEW_HPP
#define ADBLOCK_DASH_SUBSCRIPTIONS_VIEW_HPP

#include "core/settings.hpp"
#include "subscriptions_model.hpp"

#include <stream9/qt/abstract_widget_list.hpp>

class QAbstractItemModel;
class QAction;
class QModelIndex;
class QWidget;

namespace adblock_dash { namespace setting_dialog {

class FilterSetsActions;

class SubscriptionsView : public stream9::qt::AbstractWidgetList
{
    using Base = stream9::qt::AbstractWidgetList;
public:
    SubscriptionsView(Settings::Subscriptions&,
                      FilterSetsActions&,
                      QAction &toggleRightPanel,
                      QWidget* const parent = nullptr);

private:
    // @override AbstractWidgetList
    QWidget &createItemWidget(const QModelIndex&) override;

private:
    SubscriptionsModel m_model;
    FilterSetsActions &m_actions;
    QAction &m_toggleRightPanel;
};

}} // namespace adblock_dash::setting_dialog

#endif // ADBLOCK_DASH_SUBSCRIPTIONS_VIEW_HPP
