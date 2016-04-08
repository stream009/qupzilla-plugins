#include "custom_filter_set_widget.hpp"

#include "custom_filter_sets_model.hpp"

#include <cassert>

#include <QtCore/QAbstractItemModel>
#include <QtCore/QModelIndex>
#include <QtGui/QTextDocument>
#include <QtWidgets/QBoxLayout>

namespace adblock_dash { namespace setting_dialog {

CustomFilterSetWidget::
CustomFilterSetWidget(QAbstractItemModel &model, const QModelIndex &index,
                      QWidget &parent)
    : Base { &parent }
    , m_title { new QLabel { this } }
    , m_titleEdit { new QLineEdit { this } }
    , m_model { model }
    , m_index { index }
{
    this->connect(&m_model, SIGNAL(dataChanged(const QModelIndex&,
                                               const QModelIndex&)),
                  this,       SLOT(onDataChanged(const QModelIndex&,
                                                 const QModelIndex&)));
    initWidgets();
    layoutWidgets();
    connectWidgets();

    validate();
}

void CustomFilterSetWidget::
editTitle()
{
    QTextDocument doc;
    doc.setHtml(m_title->text());
    m_titleEdit->setText(doc.toPlainText());

    m_title->setVisible(false);
    m_titleEdit->setVisible(true);
    m_titleEdit->setFocus();
    m_titleEdit->selectAll();
}

void CustomFilterSetWidget::
initWidgets()
{
    const auto &title = m_model.data(m_index);
    m_title->setText(title.toString());

    m_titleEdit->setVisible(false);
}

void CustomFilterSetWidget::
layoutWidgets()
{
    auto* const layout = new QHBoxLayout { this };
    assert(layout);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->addWidget(m_title);
    layout->addWidget(m_titleEdit);
}

void CustomFilterSetWidget::
connectWidgets()
{
    this->connect(m_titleEdit, SIGNAL(editingFinished()),
                  this,          SLOT(onEditingTitleFinished()));
}

void CustomFilterSetWidget::
onDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    if (topLeft.row()     < m_index.row()) return;
    if (bottomRight.row() > m_index.row()) return;

    setValues();
}

void CustomFilterSetWidget::
onEditingTitleFinished()
{
    m_model.setData(m_index, m_titleEdit->text());

    m_title->setVisible(true);
    m_titleEdit->setVisible(false);
}

void CustomFilterSetWidget::
setValues()
{
    using Column = CustomFilterSetsModel::Column;

    const auto &title =
        m_model.index(m_index.row(), static_cast<int>(Column::Name));
    assert(title.isValid());
    setTitle(title.data().toString());
}

void CustomFilterSetWidget::
setTitle(const QString &title)
{
    m_title->setText(tr("<b>%1</b>").arg(title));
}

}} // namespace adblock_dash::setting_dialog
