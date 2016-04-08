#include "right_panel.hpp"

#include "filter_list_model.hpp"
#include "filter_list_actions.hpp"

namespace adblock_dash { namespace setting_dialog {

RightPanel::
    RightPanel(FilterListViewer &viewer, QAction &toggleFindBar,
                                         QWidget &parent)
    : Base { &parent }
    , m_find { new QPushButton { this } }
    , m_filterAction { new QPushButton { this } }
    , m_addFilter { new QPushButton { this } }
    , m_viewer { viewer }
    , m_toggleFindBar { toggleFindBar }
{
    assert(m_find);
    assert(m_filterAction);
    assert(m_addFilter);

    initWidgets();
    layoutWidgets();
    connectWidgets();
}

QSize RightPanel::
sizeHint() const
{
    return {
        this->isVisible() ? 500 : 0,
        Base::sizeHint().height()
    };
}

void RightPanel::
setVisible(const bool visible)
{
    Base::setVisible(visible);
    m_toggleFindBar.setEnabled(visible);
}

void RightPanel::
changeState(const FilterListViewer::State newState)
{
    using State = FilterListViewer::State;
    switch (newState) {
    case State::Idle:
        m_find        ->setEnabled(false);
        m_filterAction->setEnabled(false);
        m_addFilter   ->setEnabled(false);
        break;
    case State::View:
        m_find        ->setEnabled(true);
        m_filterAction->setEnabled(true);
        m_addFilter   ->setEnabled(false);
        break;
    case State::Edit:
        m_find        ->setEnabled(true);
        m_filterAction->setEnabled(true);
        m_addFilter   ->setEnabled(true);
        break;
    default:
        assert(false);
        break;
    }
}

void RightPanel::
initWidgets()
{
    m_find->setText("Find");
    m_find->setEnabled(false);

    m_filterAction->setText("Filter Action");
    m_filterAction->setMenu(&m_viewer.actions().menu());
    m_filterAction->setEnabled(false);

    m_addFilter->setText("Add Filter");
    m_addFilter->setEnabled(false);
}

void RightPanel::
layoutWidgets()
{
    auto* const top = new QHBoxLayout;
    assert(top);

    top->setContentsMargins(0, 0, 0, 0);
    top->addStretch();
    top->addWidget(m_find);
    top->addWidget(m_filterAction);
    top->addWidget(m_addFilter);

    auto* const layout = new QVBoxLayout { this };
    assert(layout);
    layout->setContentsMargins(0, 0, 0, 0);

    layout->addLayout(top);
    layout->addWidget(&m_viewer.view());
}

void RightPanel::
connectWidgets()
{
    this->connect(m_find,           SIGNAL(clicked(bool)),
                  &m_toggleFindBar,   SLOT(trigger()));

    this->connect(m_addFilter,         SIGNAL(clicked(bool)),
                  &m_viewer.actions(),   SLOT(add()));

    this->connect(
        &m_viewer, SIGNAL(stateChanged(const FilterListViewer::State)),
             this,   SLOT(changeState(const FilterListViewer::State))
    );
}

}} // namespace adblock_dash::setting_dialog
