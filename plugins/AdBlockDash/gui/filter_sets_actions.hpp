#ifndef ADBLOCK_DASH_FILTER_SETS_ACTIONS_HPP
#define ADBLOCK_DASH_FILTER_SETS_ACTIONS_HPP

#include <QtCore/QObject>
#include <QtWidgets/QAction>

class QWidget;

namespace stream9 { namespace qt {
class AbstractWidgetList;
}} // namespace stream9::qt

namespace adblock_dash { namespace setting_dialog {

class FilterSetsActions : public QObject
{
    Q_OBJECT
    using AbstractWidgetList = stream9::qt::AbstractWidgetList;
public:
    FilterSetsActions();

    // initializer
    void setWidgetList(AbstractWidgetList&);

    // query
    QAction &deleteAction() { return m_delete; }
    QAction &moveUpAction() { return m_moveUp; }
    QAction &moveDownAction() { return m_moveDown; }

    // command
    void addToWidget(QWidget&);
    void setFilterSetEnabled(const size_t, const bool);

Q_SIGNALS:
    void deleteFilterSet(const size_t);
    void moveFilterSet(const size_t from, const size_t to);
    void enableFilterSet(const size_t row, const bool enabled);

private:
    void setupActions();

private Q_SLOTS:
    void onDelete();
    void onMoveUp();
    void onMoveDown();

private:
    AbstractWidgetList *m_list = nullptr;
    QAction m_delete   { nullptr };
    QAction m_moveUp   { nullptr };
    QAction m_moveDown { nullptr };
};

}} // namespace adblock_dash::setting_dialog

#endif // ADBLOCK_DASH_FILTER_SETS_ACTIONS_HPP
