#ifndef ADBLOCK_DASH_FILTER_SET_WIDGET_HPP
#define ADBLOCK_DASH_FILTER_SET_WIDGET_HPP

#include "filter_set_actions.hpp"

#include <cassert>

#include <QtCore/QPersistentModelIndex>
#include <QtWidgets/QCheckBox>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

class QContextMenuEvent;
class QModelIndex;

class AbstractWidgetList;

namespace adblock_dash { namespace setting_dialog {

class FilterSetWidget : public QWidget
{
    Q_OBJECT
    using Base = QWidget;
public:
    //constructor
    FilterSetWidget(AbstractWidgetList&, const QModelIndex&,
                    QWidget &content,
                    FilterSetsActions&,
                    QAction &toggleRightPanel);

    // query
    const QModelIndex &index() const { return m_index; }
    QAbstractItemModel &model();

Q_SIGNALS:
    void editTitle();

protected:
    // @override QObject
    bool eventFilter(QObject* const, QEvent* const) override;

private:
    // @override QWidget
    void contextMenuEvent(QContextMenuEvent* const) override;
    void mouseDoubleClickEvent(QMouseEvent* const) override;

    void initWidgets();
    void layoutWidgets();
    void connectWidgets();
    void update();

private Q_SLOTS:
    void onDataChanged(const QModelIndex&, const QModelIndex&);

private:
    void validate() const
    {
        assert(m_checkBox && m_checkBox->parent() == this);
        assert(m_button && m_button->parent() == this);
        assert(m_content && m_content->parent() == this);
        assert(m_index.isValid());
    }

private:
    QCheckBox   *m_checkBox;
    QPushButton *m_button;
    QWidget     *m_content;

    FilterSetActions m_actions;
    AbstractWidgetList &m_list;
    QPersistentModelIndex m_index;
};

}} // namespace adblock_dash::setting_dialog

#endif // ADBLOCK_DASH_FILTER_SET_WIDGET_HPP
