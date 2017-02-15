#ifndef DOWNLOAD_DASH_PLUGIN_HPP
#define DOWNLOAD_DASH_PLUGIN_HPP

#include <QtCore/QObject>

#include <plugininterface.h>

class QString;

namespace download_dash {

class Plugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)
    Q_PLUGIN_METADATA(IID "QupZilla.Browser.plugin.DownloadDash")
public:
    Plugin() = default;

    // @override PluginInterface
    PluginSpec pluginSpec() override;
    void init(InitState state, QString const& settingsPath) override;
    void unload() override;
    bool testPlugin() override;
    void aboutToDownload(QNetworkReply const&,
                         DownloadManager::DownloadInfo&) override;
};

} // namespace download_dash

#endif // DOWNLOAD_DASH_PLUGIN_HPP
