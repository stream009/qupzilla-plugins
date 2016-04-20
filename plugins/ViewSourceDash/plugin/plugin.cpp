#include "plugin.hpp"

#include "core/view_source.hpp"
#include "dialog/setting_dialog.hpp"
#include "dialog/setting_dialog_interface.hpp"

#include <cassert>
#include <memory>

#include <boost/make_unique.hpp>

#include <QtCore/QPluginLoader>
#include <QtCore/QString>
#include <QtGui/QIcon>
#include <QtWebKitWidgets/QWebHitTestResult>
#include <QtWidgets/QAction>
#include <QtWidgets/QMenu>

#include <browserwindow.h>
#include <webview.h>
#include <pluginproxy.h>
#include <mainapplication.h>

namespace view_source_dash {

PluginSpec Plugin::
pluginSpec()
{
    PluginSpec spec;
    spec.name = "View Source Dash";
    spec.info = "View source code";
    spec.description = "Menu action to view page source little flexibly.";
    spec.version = "0.1";
    spec.author = QString::fromUtf8("stream9 <stream009@gmail.com>");
    spec.hasSettings = true;

    return spec;
}

void Plugin::
init(InitState /*state*/, const QString &settingsPath)
{
    m_settings = boost::make_unique<Settings>(settingsPath);
    assert(m_settings);

    m_processManager = boost::make_unique<ProcessManager>(*m_settings);
    assert(m_processManager);
}

void Plugin::unload() {}

bool Plugin::
testPlugin()
{
    // Require the version that the plugin was built with
    return (Qz::VERSION == QLatin1String(QUPZILLA_VERSION));
}

void Plugin::
showSettings(QWidget* const parent/*= nullptr*/)
{
    assert(parent);
    assert(m_settings);

    QPluginLoader loader { m_settings->dialogPluginPath() };

    auto* const plugin =
            dynamic_cast<SettingDialogInterface*>(loader.instance());
    if (!plugin) {
        qDebug() << "Fail to load setting dialog: " << loader.errorString();
        return;
    }

    auto &&dialog = plugin->create(*m_settings, *parent);
    assert(dialog);

    dialog->exec();
    dialog.reset();

    loader.unload();
}

void Plugin::
populateWebViewMenu(QMenu* const menu, WebView* const view,
                    const QWebHitTestResult &hit)
{
    assert(menu);
    assert(view);
    assert(m_processManager);

    if (hit.isContentEditable()) return;

    if (!hit.imageUrl().isEmpty() || !hit.linkUrl().isEmpty() ||
                                     !hit.mediaUrl().isEmpty())
    {
        return;
    }

    auto* const frame = hit.frame();
    assert(frame);

    auto* const action = new ViewSource(*frame, *m_processManager);
    assert(action);

    // find built-in "Show source code" menu item by matching icon
    const auto &icon = action->icon();
    assert(!icon.isNull());

    auto &&actions = menu->actions();
    const auto it = std::find_if(actions.begin(), actions.end(),
        [&icon](QAction* const a) {
            const auto &i = a->icon();
            return !i.isNull() && i.cacheKey() == icon.cacheKey();
        }
    );

    // replace it with new menu item
    if (it == actions.end()) {
        menu->addAction(action);
    }
    else {
        menu->insertAction(*it, action);
        menu->removeAction(*it);
    }
}

} // namespace view_source_dash
