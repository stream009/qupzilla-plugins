#ifndef ADBLOCK_DASH_SETTING_DIALOG_FILTER_LIST_VIEW_HPP
#define ADBLOCK_DASH_SETTING_DIALOG_FILTER_LIST_VIEW_HPP

#include "filter_list_actions.hpp"

#include <memory>

#include <QtWidgets/QTableView>

class QContextMenuEvent;
class QFocusEvent;
class QMenu;
class QPoint;
class QShowEvent;
class QWidget;

namespace adblock_dash { namespace setting_dialog {

class FilterListActions;

class FilterListView : public QTableView
{
    Q_OBJECT
    using Base = QTableView;
public:
    FilterListView(QWidget* const parent);

    // @override QAbstractItemView
    void setModel(QAbstractItemModel* const) override;

Q_SIGNALS:
    void modelChanged();
    void contextMenuRequested(const QPoint&);

private:
    // @override QWidget
    void contextMenuEvent(QContextMenuEvent* const) override;
    void showEvent(QShowEvent* const) override;
};

}} // namespace adblock_dash::setting_dialog

#endif // ADBLOCK_DASH_SETTING_DIALOG_FILTER_LIST_VIEW_HPP
