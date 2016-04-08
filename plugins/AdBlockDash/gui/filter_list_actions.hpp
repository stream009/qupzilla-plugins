#ifndef ADBLOCK_DASH_SETTING_DIALOG_FILTER_ACTIONS_HPP
#define ADBLOCK_DASH_SETTING_DIALOG_FILTER_ACTIONS_HPP

#include <QtCore/QObject>
#include <QtCore/QModelIndex>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>

class QItemSelection;
class QPoint;
class QString;

namespace adblock_dash { namespace setting_dialog {

class FilterListView;

class FilterListActions : public QObject
{
    Q_OBJECT
public:
    FilterListActions(FilterListView&);

    // accessor
    QMenu &menu() { return m_menu; }

    // query
    bool search(const QString &keyword,
                const Qt::CaseSensitivity = Qt::CaseInsensitive,
                const int step = +1,
                const int startOffset = 0);
public Q_SLOTS:
    void add();

private:
    void setupActions();
    void setupMenu();
    void installActions();

private Q_SLOTS:
    void onClipboardChanged();
    void onEdit();
    void onCut();
    void onCopy();
    void onPaste();
    void onDelete();
    void onModelChanged();
    void onSelectionChanged(const QItemSelection &current,
                            const QItemSelection &previous);
    void onContextMenuRequested(const QPoint&);

private:
    bool isEditable();
    QModelIndex newRow();

private:
    FilterListView &m_listView;
    QAction m_edit           { nullptr };
    QAction m_cut            { nullptr };
    QAction m_copy           { nullptr };
    QAction m_paste          { nullptr };
    QAction m_delete         { nullptr };
    QAction m_selectAll      { nullptr };
    QAction m_resetHitStatus { nullptr };
    QAction m_view           { nullptr };

    QMenu m_menu;
};

}} // namespace adblock_dash::setting_dialog

#endif // ADBLOCK_DASH_SETTING_DIALOG_FILTER_ACTIONS_HPP
