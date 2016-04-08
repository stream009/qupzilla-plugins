#include "custom_filter_sets_tab.hpp"

#include "filter_list_viewer.hpp"

#include <boost/filesystem.hpp>

#include <QtWidgets/QBoxLayout>

namespace adblock_dash { namespace setting_dialog {

CustomFilterSetsTab::
CustomFilterSetsTab(const Settings &settings,
                    CustomFilterSetsViewer &filterSetsViewer, QWidget &parent)
    : Base { &parent }
    , m_add { new QPushButton { this } }
    , m_settings { settings }
    , m_filterSetsViewer { filterSetsViewer }
{
    assert(m_add);

    initWidgets();
    layoutWidgets();
    connectWidgets();

    validate();
}

void CustomFilterSetsTab::
initWidgets()
{
    m_add->setText(tr("Add filter group"));
}

void CustomFilterSetsTab::
layoutWidgets()
{
    auto* const top = new QHBoxLayout;
    assert(top);
    top->setContentsMargins(0, 0, 0, 0);
    top->addStretch();
    top->addWidget(m_add);

    auto* const layout = new QVBoxLayout { this };
    assert(layout);
    layout->addLayout(top);
    layout->addWidget(&m_filterSetsViewer.widget());

    assert(top->parent() == layout);
}

void CustomFilterSetsTab::
connectWidgets()
{
    this->connect(m_add, SIGNAL(clicked()),
                  this,    SLOT(addFilter()));
}

void CustomFilterSetsTab::
addFilter()
{
    namespace bfs = boost::filesystem;

    const auto &name = m_settings.newFilterSetName();
    const auto &directory = m_settings.directory().value().toLocal8Bit();
    const auto &path =
        bfs::path { directory.constData() } / (bfs::unique_path() += ".txt");

    m_filterSetsViewer.newFilterSet(name, bfs::absolute(path));
}

}} // namespace adblock_dash::setting_dialog
