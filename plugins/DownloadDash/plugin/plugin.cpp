#include "plugin.hpp"

#include <cassert>

#include <QtCore/QString>
#include <QtCore/QMessageLogContext>
#include <QtNetwork/QNetworkReply>

namespace download_dash {

PluginSpec Plugin::
pluginSpec()
{
    PluginSpec spec;
    spec.name = "Download Dash";
    spec.info = "Download helper";
    spec.description = "Memorize download dialog option.";
    spec.version = "0.1";
    spec.author = QString::fromUtf8("stream9 <stream009@gmail.com>");
    spec.hasSettings = false;

    return spec;
}

void Plugin::
init(InitState /*state*/, QString const& /*settingsPath*/)
{
}

void Plugin::unload()
{
}

bool Plugin::
testPlugin()
{
    // Require the version that the plugin was built with
    return (Qz::VERSION == QLatin1String(QUPZILLA_VERSION));
}

static QString
getFilename(QUrl const& url, QString const& suggestedFilename)
{
    assert(!url.isEmpty());

    QString result;

    if (!suggestedFilename.isEmpty()) {
        result = suggestedFilename;
    }
    else {
        auto const& path = url.path();
        if (!path.isEmpty()) {
            auto const i = path.lastIndexOf('/');
            result = i == -1 ? path : path.mid(i+1);
        }
        else {
            result = "index.html";
        }
    }

    assert(!result.isEmpty());
    return result;
}

void Plugin::aboutToDownload(QNetworkReply const& reply,
                             DownloadManager::DownloadInfo& info)
{
#if 0
    qDebug() << "aboutToDownload:\n"
             << "URL: " << reply.url().toDisplayString();
    for (auto const& header: reply.rawHeaderPairs()) {
        qDebug() << header.first << ":" << header.second;
    }
#endif
    auto const& url = reply.url();
    assert(!url.isEmpty());

    auto const& filename = getFilename(url, info.suggestedFileName);

    if (filename.endsWith("jpg")) {
        info.preferredOption = DownloadManager::OpenFile;
        info.askWhatToDo = false;
    }
    else if (filename.endsWith("mkv") || filename.endsWith("mp4")) {
        info.preferredOption = DownloadManager::ExternalManager;
        info.askWhatToDo = false;
    }
    else if (filename.endsWith(".torrent")) {
        info.preferredOption = DownloadManager::OpenFile;
        info.askWhatToDo = false;
    }
}

} // namespace download_dash
