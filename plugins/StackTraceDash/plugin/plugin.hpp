#ifndef STACK_TRACE_DASH_PLUGIN_HPP
#define STACK_TRACE_DASH_PLUGIN_HPP

#include <QtCore/QObject>

#include <plugininterface.h>

class QString;

namespace stack_trace_dash {

class Plugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)
    Q_PLUGIN_METADATA(IID "QupZilla.Browser.plugin.StackTraceDash")
public:
    Plugin() = default;

    // @override PluginInterface
    PluginSpec pluginSpec() override;
    void init(InitState state, QString const& settingsPath) override;
    void unload() override;
    bool testPlugin() override;
};

} // namespace stack_trace_dash

#endif // STACK_TRACE_DASH_PLUGIN_HPP
