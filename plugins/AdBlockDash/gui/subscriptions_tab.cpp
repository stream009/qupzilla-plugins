#include "subscriptions_tab.hpp"

#include "filter_list_viewer.hpp"

namespace adblock_dash { namespace setting_dialog {

SubscriptionsTab::
SubscriptionsTab(const Settings &settings,
                 SubscriptionsViewer &subscriptionsViewer,
                 QWidget &parent)
    : Base { &parent }
    , m_settings { settings }
    , m_subscriptionsViewer { subscriptionsViewer }
    , m_add { new QPushButton { this } }
    , m_allowNonIntrusiveAds { new QCheckBox { this } }
    , m_viewList { new QLabel { this } }
    , m_readMore { new QLabel { this } }
{
    assert(m_add);
    assert(m_allowNonIntrusiveAds);
    assert(m_viewList);
    assert(m_readMore);

    initWidgets();
    layoutWidgets();
    connectWidgets();

    validate();
}

void SubscriptionsTab::
initWidgets()
{
    m_add->setText("Add filter subscription...");

    const auto &exceptionRules = m_settings.exceptionRules();

    m_allowNonIntrusiveAds->setText("Allow some non-intrusive advertising");
    m_allowNonIntrusiveAds->setCheckState(
                    exceptionRules.enabled() ? Qt::Checked : Qt::Unchecked);

    m_viewList->setText(QString { "<a href=\"%1\">View list</a>" }
                               .arg(exceptionRules.location().toString()));
    m_readMore->setText(QString { "<a href=\"%1\">Read more</a>" }
                                      .arg(m_settings.acceptableAdsUrl()));
}

void SubscriptionsTab::
layoutWidgets()
{
    auto* const top = new QHBoxLayout;
    assert(top);
    top->setContentsMargins(0, 0, 0, 0);
    top->addStretch();
    top->addWidget(m_add);

    auto* const bottom = new QHBoxLayout;
    assert(bottom);
    bottom->setContentsMargins(0, 0, 0, 0);
    bottom->addWidget(m_allowNonIntrusiveAds);
    bottom->addWidget(m_viewList);
    bottom->addWidget(m_readMore);
    bottom->addStretch();

    auto* const layout = new QVBoxLayout { this };
    assert(layout);

    layout->addLayout(top);
    layout->addWidget(&m_subscriptionsViewer.widget());
    layout->addLayout(bottom);
}

void SubscriptionsTab::
connectWidgets()
{
    this->connect(m_add, SIGNAL(clicked(bool)),
                  this,    SLOT(onAddClicked()));

    this->connect(
        m_viewList, SIGNAL(linkHovered(const QString&)),
        this,         SLOT(onLinkHovered(const QString&)));
    this->connect(
        m_viewList, SIGNAL(linkActivated(const QString&)),
        this,         SLOT(onLinkActivated(const QString&)));
    this->connect(
        m_readMore, SIGNAL(linkHovered(const QString&)),
        this,         SLOT(onLinkHovered(const QString&)));
    this->connect(
        m_readMore, SIGNAL(linkActivated(const QString&)),
        this,         SLOT(onLinkActivated(const QString&)));

    this->connect(
        m_allowNonIntrusiveAds, SIGNAL(stateChanged(int)),
        this,                    SLOT(onAllowNonIntrusiveAdsChanged(const int)));
}

void SubscriptionsTab::
onAddClicked()
{
    const QUrl &url { m_settings.knownSubscriptionsUrl() };
    assert(url.isValid());

    m_subscriptionsViewer.openUrl(url);
}

void SubscriptionsTab::
onLinkHovered(const QString &url)
{
    if (url.isNull()) return; // It happens somehow

    auto* const widget = dynamic_cast<QWidget*>(this->sender());
    assert(widget);
    widget->setToolTip(url);
}

void SubscriptionsTab::
onLinkActivated(const QString &url)
{
    assert(!url.isNull());

    m_subscriptionsViewer.openUrl(url);
}

void SubscriptionsTab::
onAllowNonIntrusiveAdsChanged(const int state)
{
    m_subscriptionsViewer.allowNonIntrusiveAds(state == Qt::Checked);
}

}} // namespace adblock_dash::setting_dialog
