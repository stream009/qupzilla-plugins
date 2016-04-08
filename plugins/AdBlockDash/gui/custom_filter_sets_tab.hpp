#ifndef ADBLOCK_DASH_SETTING_DIALOG_CUSTOM_FILTERS_SET_TAB_HPP
#define ADBLOCK_DASH_SETTING_DIALOG_CUSTOM_FILTERS_SET_TAB_HPP

#include "common/path.hpp"
#include "core/settings.hpp"
#include "custom_filter_sets_view.hpp"
#include "custom_filter_sets_viewer.hpp"

#include <cassert>

#include <boost/filesystem/path.hpp>

#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

class QPushButton;

namespace adblock_dash {

class CustomFilterSetsView;
class Settings;

namespace setting_dialog {

class CustomFilterSetsViwer;
class FilterListViewer;

class CustomFilterSetsTab : public QWidget
{
    Q_OBJECT

    using Base = QWidget;
public:
    CustomFilterSetsTab(const Settings &,
                        CustomFilterSetsViewer&, QWidget &parent);

private:
    void initWidgets();
    void layoutWidgets();
    void connectWidgets();

private Q_SLOTS:
    void addFilter();

private:
    void validate() const
    {
        assert(m_add && m_add->parent() == this);
        assert(m_filterSetsViewer.widget().parent() == this);
    }

private:
    QPushButton *m_add;

    const Settings &m_settings;
    CustomFilterSetsViewer &m_filterSetsViewer;
};

}} // namespace adblock_dash::setting_dialog

#endif // ADBLOCK_DASH_SETTING_DIALOG_CUSTOM_FILTERS_SET_TAB_HPP
