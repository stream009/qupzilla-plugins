#ifndef ADBLOCK_DASH_FILTER_SET_ACTIONS_HPP
#define ADBLOCK_DASH_FILTER_SET_ACTIONS_HPP

#include <QtCore/QObject>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>

class QWidget;

namespace adblock_dash { namespace setting_dialog {

class FilterSetWidget;
class FilterSetsActions;

class FilterSetActions : public QObject
{
    Q_OBJECT
public:
    FilterSetActions(FilterSetWidget &item,
                     FilterSetsActions&, QAction &toggleRightPanel);

    QMenu &menu() { return m_menu; }

public Q_SLOTS:
    void setCheckState(const int);

Q_SIGNALS:
    void editTitle();

private:
    void setupActions();
    void setupMenu();

private:
    FilterSetWidget &m_item;
    FilterSetsActions &m_filterSetsActions;
    QAction &m_toggleRightPanel;
    QAction m_editTitle       { nullptr };

    QMenu m_menu;
};

}} // namespace adblock_dash::setting_dialog

#endif // ADBLOCK_DASH_FILTER_SET_ACTIONS_HPP
