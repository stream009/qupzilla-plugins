#include "custom_filter_sets_view.hpp"

#include "custom_filter_set_widget.hpp"
#include "filter_set_widget.hpp"
#include "filter_sets_actions.hpp"

#include <QtCore/QModelIndex>

namespace adblock_dash { namespace setting_dialog {

CustomFilterSetsView::
CustomFilterSetsView(Settings::FilterSets &filterSets,
                     FilterSetsActions &actions,
                     QAction &toggleRightPanel,
                     QWidget* const parent/*= nullptr*/)
    : Base { parent }
    , m_model { filterSets }
    , m_actions { actions }
    , m_toggleRightPanel { toggleRightPanel }
{
    this->setModel(&m_model);
}

QWidget &CustomFilterSetsView::
createItemWidget(const QModelIndex &index)
{
    auto* const content = new CustomFilterSetWidget { m_model, index, *this };
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
