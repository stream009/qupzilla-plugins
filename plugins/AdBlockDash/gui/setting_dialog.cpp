#include "setting_dialog.hpp"

#include "core/settings.hpp"
#include <common/path.hpp>

#include <QtGui/QCloseEvent>
#include <QtWidgets/QWidget>

namespace adblock_dash { namespace setting_dialog {

SettingDialog::
SettingDialog(Settings &settings, QWidget &parent)
    : Base         { &parent }
    , m_subscriptionsViewer { settings, m_toggleRightPanel }
    , m_filterSetsViewer { settings.customFilterSets(),
                           m_toggleRightPanel }
    , m_filterListViewer { settings }
    , m_splitter   { new QSplitter { this } }
    , m_leftPanel  { new LeftPanel { settings,
                                     m_subscriptionsViewer,
                                     m_filterSetsViewer,
                                     *this } }
    , m_rightPanel { new RightPanel { m_filterListViewer,
                                      m_toggleFindBar, *this } }
    , m_findBar    { new FindBar {
                          m_filterListViewer.actions(), *this } }
    , m_buttonBox  { new QDialogButtonBox { this } }
{
    assert(m_splitter);
    assert(m_leftPanel);
    assert(m_rightPanel);
    assert(m_findBar);
    assert(m_buttonBox);

    setupActions();
    initWidgets();
    layoutWidgets();
    connectWidgets();

    this->connect(&m_subscriptionsViewer, SIGNAL(selected(const Path, const bool)),
                  &m_filterListViewer,      SLOT(setPath(const Path, const bool)));
    this->connect(&m_subscriptionsViewer, SIGNAL(selectionCleared()),
                  &m_filterListViewer,      SLOT(clear()));
    this->connect(&m_subscriptionsViewer, SIGNAL(requestReload()),
                  this,                     SLOT(onReloadRequested()));
    this->connect(&m_subscriptionsViewer, SIGNAL(openUrlRequest(const QUrl&)),
                  this,                   SIGNAL(openUrlRequest(const QUrl&)));

    this->connect(&m_filterSetsViewer, SIGNAL(selected(const Path, const bool)),
                  &m_filterListViewer,   SLOT(setPath(const Path, const bool)));
    this->connect(&m_filterSetsViewer, SIGNAL(selectionCleared()),
                  &m_filterListViewer,   SLOT(clear()));
    this->connect(&m_filterSetsViewer, SIGNAL(requestReload()),
                  this,                  SLOT(onReloadRequested()));

    this->connect(&m_filterListViewer, SIGNAL(requestReload()),
                  this,                  SLOT(onReloadRequested()));

    qRegisterMetaType<Path>(); //TODO move to somewhere

    validate();
}

void SettingDialog::
done(const int result)
{
    m_filterListViewer.saveFileIfNecessary();

    if (m_needReload) {
        qDebug() << "reload required";
    }

    Base::done(result);
}

void SettingDialog::
initWidgets()
{
    this->setWindowTitle(tr("ADBlock Setting"));
    this->setMinimumSize(0, 400);

    m_rightPanel->setVisible(false);

    m_findBar->setVisible(false);

    m_buttonBox->setOrientation(Qt::Horizontal);
    m_buttonBox->setStandardButtons(QDialogButtonBox::Ok);
}

void SettingDialog::
layoutWidgets()
{
    m_splitter->addWidget(m_leftPanel);
    m_splitter->addWidget(m_rightPanel);

    auto* const layout = new QVBoxLayout { this };
    assert(layout);

    layout->addWidget(m_splitter,  1);
    layout->addWidget(m_findBar,   0);
    layout->addWidget(m_buttonBox, 0);
}

void SettingDialog::
connectWidgets()
{
    this->connect(m_buttonBox, SIGNAL(accepted()),
                  this,          SLOT(accept()));
    this->connect(m_buttonBox, SIGNAL(rejected()),
                  this,          SLOT(reject()));
}

void SettingDialog::
setupActions()
{
    m_toggleRightPanel.setText(tr("Show/Hide Filters"));
    m_toggleRightPanel.setShortcut(Qt::CTRL + Qt::Key_R);
    this->addAction(&m_toggleRightPanel);

    this->connect(&m_toggleRightPanel, SIGNAL(triggered()),
                  this,                  SLOT(onToggleRightPanel()));

    m_toggleFindBar.setText(tr("Show/Hide Find Bar"));
    m_toggleFindBar.setShortcut(Qt::CTRL + Qt::Key_F);
    this->addAction(&m_toggleFindBar);

    this->connect(&m_toggleFindBar, SIGNAL(triggered()),
                  this,               SLOT(onToggleFindBar()));
}

void SettingDialog::
adjustSize()
{
    const auto &margin = this->layout()->contentsMargins();
    const auto newWidth =
        margin.left() + margin.right() +
        m_leftPanel->sizeHint().width() +
        m_splitter->handleWidth() +
        m_rightPanel->sizeHint().width();

    this->setMinimumSize({ 0, 0 });
    this->resize(newWidth, this->height());

    m_splitter->setSizes(QList<int>{}
            << m_leftPanel->sizeHint().width()
            << m_rightPanel->sizeHint().width());
}

void SettingDialog::
onToggleRightPanel()
{
    const auto &visible = !m_rightPanel->isVisible();

    m_rightPanel->setVisible(visible);

    adjustSize();

    if (!visible) {
        m_findBar->hide();
    }
}

void SettingDialog::
onToggleFindBar()
{
    const auto visible = !m_findBar->isVisible();
    m_findBar->setVisible(visible);

    if (visible) {
        m_findBar->setFocus();
    }
}

void SettingDialog::
onReloadRequested()
{
    m_needReload = true;
}

}} // namespace adblock_dash::setting_dialog
