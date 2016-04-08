#include "plugin.hpp"

#include "blocked_network_reply.hpp"
#include "scheme_handler.hpp"

#include "common/utility.hpp"
#include "core/adblock_dash.hpp"
#include "core/settings.hpp"
#include "gui/setting_dialog.hpp"

#include <cassert>

#include <QtCore/QTranslator>

#include <browserwindow.h>
#include <mainapplication.h>
#include <pluginproxy.h>
#include <popupwebpage.h>
#include <qzcommon.h>
#include <tabbedwebview.h>
#include <tabwidget.h>
#include <webpage.h>
#include <webview.h>

namespace adblock_dash {

Plugin::~Plugin() = default;

PluginSpec Plugin::
pluginSpec()
{
    PluginSpec spec;

    spec.name = "AdBlock Dash";
    spec.info = "Alternative AdBlock engine.";
    spec.description = "";
    spec.version = "0.0.1";
    spec.author = "stream9 <stream009@gmail.com>";
    spec.icon = QPixmap(":qupzilla.png");
    spec.hasSettings = true;

    return spec;
}

void Plugin::
init(InitState state, const QString &settingsPath)
{
    assert(mApp);

    m_settings.reset(new Settings(settingsPath));
    assert(m_settings);

    auto* const manager = mApp->networkManager();
    assert(manager);

    m_adBlockDash.reset(new AdBlockDash(*m_settings, *manager));
    assert(m_adBlockDash);

    m_schemeHandler.reset(new SchemeHandler { *manager, *m_adBlockDash });
    assert(m_schemeHandler);

    auto* const plugins = mApp->plugins();
    assert(plugins);

    this->connect(
        plugins,             SIGNAL(webPageCreated(WebPage*)),
        m_adBlockDash.get(),   SLOT(onWebPageCreated(WebPage*)));

    if (state == LateInitState) {
        for (auto* const window: mApp->windows()) {
            assert(window);
            auto* const tabWidget = window->tabWidget();
            assert(tabWidget);

            for (int i = 0, cnt = tabWidget->count(); i < cnt; ++i) {
                auto* const tab = dynamic_cast<WebTab*>(tabWidget->widget(i));
                assert(tab);
                auto* const webView = tab->webView();
                assert(webView);
                m_adBlockDash->onWebPageCreated(webView->page());
            }
        }
    }

    assert(m_settings);
    assert(m_adBlockDash);
    assert(m_schemeHandler);
}

void Plugin::
unload()
{
    // I don't know why, but QupZilla doesn't call the destructor.
    // So, we have to release resources here.
    m_translator.reset(nullptr);
    m_schemeHandler.reset(nullptr);
    m_adBlockDash.reset(nullptr);
    m_settings.reset(nullptr);
}

bool Plugin::
testPlugin()
{
    return (Qz::VERSION == QL1S(QUPZILLA_VERSION));
}

QTranslator* Plugin::
getTranslator(const QString &locale)
{
    if (!m_translator) {
        m_translator.reset(new QTranslator);
        assert(m_translator);
    }

    m_translator->load(locale, ":/adblock_dash/locale/");

    return m_translator.get();
}

void Plugin::
showSettings(QWidget* const parent/*= 0*/)
{
    assert(parent);
    assert(m_settings);

    SettingDialog dialog { *m_settings, *parent };

    this->connect(&dialog, SIGNAL(openUrlRequest(const QUrl&)),
                  this,      SLOT(openUrl(const QUrl&)));
    dialog.exec();

    if (dialog.needReload()) {
        m_adBlockDash->reload();
    }
}

static void
closePopup(const QNetworkRequest &request)
{
    auto* const frame =
                dynamic_cast<QWebFrame*>(request.originatingObject());
    if (!frame) return;

    if (frame->url().toEncoded() != "about:blank") return;

    auto* const page = dynamic_cast<PopupWebPage*>(frame->page());
    if (!page) return;

    if (frame != page->mainFrame()) return;

    auto* const view = dynamic_cast<WebView*>(page->view());
    assert(view);

    view->closeView();
}

QNetworkReply * Plugin::
createRequest(const QNetworkAccessManager::Operation op,
              const QNetworkRequest &request, QIODevice*)
{
    assert(m_adBlockDash);

    if (op == QNetworkAccessManager::GetOperation) {
        if (m_adBlockDash->shouldBlock(request)) {
            closePopup(request);
            return new BlockedNetworkReply(request, "Block by AdBlock Dash");
        }
        else {
            return nullptr;
        }
    }
    else {
        return nullptr;
    }
}

void Plugin::
openUrl(const QUrl &url)
{
    assert(url.isValid());
    assert(mApp);

    auto* const window = mApp->getWindow();
    if (!window) return;

    auto* const view = window->weView();
    assert(view);

    view->openUrlInNewTab(url, Qz::NT_SelectedTab);
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(AdBlockDash, Plugin)
#endif

} // namespace adblock_dash
