#include "filter_set_actions.hpp"

#include "common/utility.hpp"
#include "filter_set_widget.hpp"
#include "filter_sets_actions.hpp"
#include "qt_widget_list/abstract_widget_list.hpp"

#include <cassert>

#include <QtWidgets/QAction>
#include <QtWidgets/QWidget>

namespace adblock_dash { namespace setting_dialog {

FilterSetActions::
FilterSetActions(FilterSetWidget &item,
                 FilterSetsActions &filterSetsActions,
                 QAction &toggleRightPanel)
    : m_item { item }
    , m_filterSetsActions { filterSetsActions }
    , m_toggleRightPanel { toggleRightPanel }
{
    setupActions();
    setupMenu();
}

void FilterSetActions::
setCheckState(const int status)
{
    const auto &index = m_item.index();

    m_filterSetsActions.setFilterSetEnabled(
                ::toSizeT(index.row()), status == Qt::Checked);
}

void FilterSetActions::
setupActions()
{
    m_editTitle.setText(tr("Edit title"));
    m_editTitle.setShortcut(Qt::Key_F2);
    m_editTitle.setShortcutContext(Qt::WidgetShortcut);

    this->connect(&m_editTitle, SIGNAL(triggered()),
                  this,         SIGNAL(editTitle()));

    m_item.addAction(&m_editTitle);
    m_filterSetsActions.addToWidget(m_item);
}

void FilterSetActions::
setupMenu()
{
    m_menu.addAction(&m_editTitle);

    m_menu.addAction(&m_toggleRightPanel);

    m_menu.addAction(&m_filterSetsActions.deleteAction());

    m_menu.addSeparator();

    m_menu.addAction(&m_filterSetsActions.moveUpAction());
    m_menu.addAction(&m_filterSetsActions.moveDownAction());
}

}} // namespace adblock_dash::setting_dialog
