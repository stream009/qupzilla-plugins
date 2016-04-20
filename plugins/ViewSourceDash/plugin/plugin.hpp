#ifndef VIEW_SOURCE_DASH_PLUGIN_HPP
#define VIEW_SOURCE_DASH_PLUGIN_HPP

#include "core/process_manager.hpp"
#include "core/settings.hpp"

#include <memory>

#include <QtCore/QObject>

#include <plugininterface.h>

class QAction;
class QMenu;
class QString;
class QWebHitTestResult;

class WebView;

namespace view_source_dash {

class ProcessManager;
class Settings;

class Plugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)
    Q_PLUGIN_METADATA(IID "QupZilla.Browser.plugin.ViewSourceDash")
public:
    Plugin() = default;

    // @override PluginInterface
    PluginSpec pluginSpec() override;
    void init(InitState state, const QString &settingsPath) override;
    void unload() override;
    bool testPlugin() override;
    void showSettings(QWidget* const parent = nullptr) override;

    void populateWebViewMenu(QMenu *, WebView *,
                             const QWebHitTestResult &) override;

private:
    std::unique_ptr<ProcessManager> m_processManager = nullptr;
    std::unique_ptr<Settings> m_settings = nullptr;
};

} // namespace view_source_dash

#endif // VIEW_SORUCE_DASH_PLUGIN_HPP
