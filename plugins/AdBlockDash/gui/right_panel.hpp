#ifndef ADBLOCK_DASH_SETTING_DIALOG_RIGHT_PANEL_HPP
#define ADBLOCK_DASH_SETTING_DIALOG_RIGHT_PANEL_HPP

#include "filter_list_view.hpp"
#include "filter_list_viewer.hpp"

#include <boost/filesystem.hpp>

#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

class QAbstractItemView;

namespace adblock_dash { namespace setting_dialog {

class FilterListModel;

class RightPanel : public QWidget
{
    Q_OBJECT

    using Base = QWidget;
    using Path = boost::filesystem::path;
public:
    RightPanel(FilterListViewer&, QAction &toggleFindBar, QWidget &parent);

    // @override QWidget
    QSize sizeHint() const override;
    void setVisible(const bool) override;

private:
    void initWidgets();
    void layoutWidgets();
    void connectWidgets();

private Q_SLOTS:
    void changeState(const FilterListViewer::State);

private:
    void validate() const
    {
        assert(m_find && m_find->parent() == this);
        assert(m_filterAction && m_filterAction->parent() == this);
        assert(m_addFilter && m_addFilter->parent() == this);
        assert(m_viewer.view().parent() == this);
    }

private:
    QPushButton *m_find;
    QPushButton *m_filterAction;
    QPushButton *m_addFilter;

    FilterListViewer &m_viewer;
    QAction &m_toggleFindBar;
};

}} // namespace adblock_dash::setting_dialog

#endif // ADBLOCK_DASH_SETTING_DIALOG_RIGHT_PANEL_HPP
