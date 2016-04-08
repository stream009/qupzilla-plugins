#ifndef ADBLOCK_DASH_SUBSCRIPTIONS_TAB_HPP
#define ADBLOCK_DASH_SUBSCRIPTIONS_TAB_HPP

#include "core/settings.hpp"
#include "common/path.hpp"
#include "subscriptions_viewer.hpp"

#include <boost/filesystem/path.hpp>

#include <QtWidgets/QCheckBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

class QUrl;

namespace adblock_dash {

namespace setting_dialog {

class FilterListViewer;
class SubscriptionsViewer;

class SubscriptionsTab : public QWidget
{
    Q_OBJECT

    using Base = QWidget;
public:
    SubscriptionsTab(const Settings&, SubscriptionsViewer &, QWidget &parent);

private:
    void initWidgets();
    void layoutWidgets();
    void connectWidgets();

private:
    Q_SLOT void onAddClicked();
    Q_SLOT void onLinkHovered(const QString&);
    Q_SLOT void onLinkActivated(const QString&);
    Q_SLOT void onAllowNonIntrusiveAdsChanged(const int);

    void validate() const
    {
        assert(m_add && m_add->parent() == this);
        assert(m_subscriptionsViewer.widget().parent() == this);
        assert(m_allowNonIntrusiveAds
                      && m_allowNonIntrusiveAds->parent() == this);
        assert(m_viewList && m_viewList->parent() == this);
        assert(m_readMore && m_readMore->parent() == this);
    }

private:
    const Settings &m_settings;
    SubscriptionsViewer &m_subscriptionsViewer;

    QPushButton *m_add;
    QCheckBox   *m_allowNonIntrusiveAds;
    QLabel      *m_viewList;
    QLabel      *m_readMore;
};

}} // namespace adblock_dash::setting_dialog

#endif // ADBLOCK_DASH_SUBSCRIPTIONS_TAB_HPP
