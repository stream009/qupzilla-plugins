#ifndef SYSLOG_DASH_PLUGIN_HPP
#define SYSLOG_DASH_PLUGIN_HPP

#include <QtCore/QObject>

#include <plugininterface.h>

class QString;

namespace syslog_dash {

class Plugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)
    Q_PLUGIN_METADATA(IID "QupZilla.Browser.plugin.SyslogDash")
public:
    Plugin() = default;

    // @override PluginInterface
    PluginSpec pluginSpec() override;
    void init(InitState state, QString const& settingsPath) override;
    void unload() override;
    bool testPlugin() override;
};

} // namespace syslog_dash

#endif // SYSLOG_DASH_PLUGIN_HPP
