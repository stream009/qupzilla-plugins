#ifndef ADBLOCK_DASH_SETTING_DIALOG_CUSTON_FILTER_SETS_VIEW_HPP
#define ADBLOCK_DASH_SETTING_DIALOG_CUSTON_FILTER_SETS_VIEW_HPP

#include "core/settings.hpp"
#include "custom_filter_sets_model.hpp"

#include <stream9/qt/abstract_widget_list.hpp>

class QModelIndex;
class QWidget;

namespace adblock_dash { namespace setting_dialog {

class FilterSetsActions;

class CustomFilterSetsView : public stream9::qt::AbstractWidgetList
{
    using Base = stream9::qt::AbstractWidgetList;
public:
    CustomFilterSetsView(Settings::FilterSets&,
                         FilterSetsActions &actions,
                         QAction &toggleRightPanel,
                         QWidget* const parent = nullptr);
private:
    // @override AbstractWidgetList
    QWidget &createItemWidget(const QModelIndex&) override;

private:
    CustomFilterSetsModel m_model;
    FilterSetsActions &m_actions;
    QAction &m_toggleRightPanel;
};

}} // namespace adblock_dash::setting_dialog

#endif // ADBLOCK_DASH_SETTING_DIALOG_CUSTON_FILTER_SETS_VIEW_HPP
