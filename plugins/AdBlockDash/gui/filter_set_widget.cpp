#include "filter_set_widget.hpp"

#include "common/utility.hpp"

#include <QtCore/QModelIndex>
#include <QtGui/QCursor>

#include <stream9/qt/abstract_widget_list.hpp>

namespace adblock_dash { namespace setting_dialog {

FilterSetWidget::
FilterSetWidget(AbstractWidgetList &list,
                const QModelIndex &index,
                QWidget &content,
                FilterSetsActions &filterSetsActions,
                QAction &toggleRightPanel)
    : Base { &list }
    , m_checkBox { new QCheckBox { this } }
    , m_button { new QPushButton { this } }
    , m_content { &content }
    , m_actions { *this, filterSetsActions, toggleRightPanel }
    , m_list { list }
    , m_index { index }
{
    assert(m_checkBox);
    assert(m_button);
    assert(m_index.isValid());

    this->setAutoFillBackground(true);

    initWidgets();
    layoutWidgets();
    connectWidgets();

    this->connect(&m_actions, SIGNAL(editTitle()),
                  this,       SIGNAL(editTitle()));
    validate();
}

QAbstractItemModel &FilterSetWidget::
model()
{
    auto* const model = m_list.model();
    assert(model);

    return *model;
}

bool FilterSetWidget::
eventFilter(QObject* const obj, QEvent* const event)
{
    assert(obj);
    assert(event);

    if (obj == m_button && event->type() == QEvent::MouseButtonPress) {
        m_list.select(::toSizeT(m_index.row()));
        return false;
    }

    return Base::eventFilter(obj, event);
}

void FilterSetWidget::
contextMenuEvent(QContextMenuEvent* const)
{
    m_actions.menu().popup(QCursor::pos());
}

void FilterSetWidget::
mouseDoubleClickEvent(QMouseEvent* const)
{
    Q_EMIT editTitle();
}

void FilterSetWidget::
initWidgets()
{
    m_checkBox->setText("");

    m_button->setText(tr("Action"));
    m_button->setMenu(&m_actions.menu());

    m_button->installEventFilter(this);

    update();
}

void FilterSetWidget::
layoutWidgets()
{
    auto* const layout = new QHBoxLayout(this);
    assert(layout && layout->parent() == this);
    layout->addWidget(m_checkBox, 0);
    layout->addWidget(m_content, 1);
    layout->addWidget(m_button, 0);
}

void FilterSetWidget::
connectWidgets()
{
    this->connect(m_checkBox, SIGNAL(stateChanged(int)),
                  &m_actions,   SLOT(setCheckState(const int)));

    this->connect(&model(), SIGNAL(dataChanged(const QModelIndex&, const QModelIndex&)),
                  this,       SLOT(onDataChanged(const QModelIndex&, const QModelIndex&)));
}

void FilterSetWidget::
update()
{
    const auto &enabled = m_index.data(Qt::CheckStateRole).toBool();
    m_checkBox->setCheckState(enabled ? Qt::Checked : Qt::Unchecked);
}

void FilterSetWidget::
onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    if (topLeft.row() > m_index.row()
                     || m_index.row() > bottomRight.row())
    {
        return;
    }

    if (topLeft.column() > m_index.column()
                        || m_index.column() > bottomRight.column())
    {
        return;
    }

    update();
}

}} // namespace adblock_dash::setting_dialog
