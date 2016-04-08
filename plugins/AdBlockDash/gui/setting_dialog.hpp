#ifndef ADBLOCK_DASH_SETTING_DIALOG_HPP
#define ADBLOCK_DASH_SETTING_DIALOG_HPP

#include <cassert>

#include "custom_filter_sets_viewer.hpp"
#include "filter_list_viewer.hpp"
#include "find_bar.hpp"
#include "left_panel.hpp"
#include "right_panel.hpp"
#include "subscriptions_viewer.hpp"

#include "common/path.hpp"

#include <QtWidgets/QAction>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QSplitter>

class QWidget;
class QCloseEvent;

namespace adblock_dash {

class AdBlockDash;
class Settings;

namespace setting_dialog {

class SettingDialog : public QDialog
{
    Q_OBJECT

    using Base = QDialog;
public:
    SettingDialog(Settings&, QWidget &parent);

    // @override QDialog
    void done(int result) override;

    bool needReload() const { return m_needReload; }

Q_SIGNALS:
    void openUrlRequest(const QUrl&);

private:
    void initWidgets();
    void layoutWidgets();
    void connectWidgets();
    void setupActions();
    void adjustSize();

private Q_SLOTS:
    void onToggleRightPanel();
    void onToggleFindBar();
    void onReloadRequested();

private:
    void validate() const
    {
        assert(m_splitter && m_splitter->parent() == this);
        assert(m_leftPanel && m_leftPanel->parent() == m_splitter);
        assert(m_rightPanel && m_rightPanel->parent() == m_splitter);
        assert(m_findBar && m_findBar->parent() == this);
        assert(m_buttonBox && m_buttonBox->parent() == this);
    }

private:
    QAction m_toggleRightPanel { nullptr };
    QAction m_toggleFindBar { nullptr };
    SubscriptionsViewer m_subscriptionsViewer;
    CustomFilterSetsViewer m_filterSetsViewer;
    FilterListViewer m_filterListViewer;

    QSplitter *m_splitter;
    LeftPanel *m_leftPanel;
    RightPanel *m_rightPanel;
    FindBar *m_findBar;
    QDialogButtonBox *m_buttonBox;

    bool m_needReload = false;
};

} // namespace setting_dialog

using setting_dialog::SettingDialog;

} // namespace adblock_dash

#endif // ADBLOCK_DASH_SETTING_DIALOG_HPP
