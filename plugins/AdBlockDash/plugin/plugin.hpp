#ifndef ADBLOCK_DASH_PLUGIN_H
#define ADBLOCK_DASH_PLUGIN_H

#include "scheme_handler.hpp"
#include "core/adblock_dash.hpp"
#include "core/settings.hpp"

#include <memory>

#include <boost/filesystem.hpp>

#include <QtCore/QObject>
#include <QtNetwork/QNetworkAccessManager>

#include <plugininterface.h>

class QIODevice;
class QNetworkReply;
class QNetworkRequest;
class QString;
class QTranslator;
class QWidget;

namespace adblock_dash {

class Plugin : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "QupZilla.Browser.plugin.AdBlockDash")
#endif
    using Path = boost::filesystem::path;
public:
    ~Plugin() override;

private:
    // @override PluginInterface.
    PluginSpec pluginSpec() override;
    void init(InitState state, const QString &settingsPath) override;
    void unload() override;
    bool testPlugin() override;
    QTranslator* getTranslator(const QString &locale) override;
    void showSettings(QWidget* parent = 0) override;

    QNetworkReply *createRequest(
            const QNetworkAccessManager::Operation,
            const QNetworkRequest&, QIODevice *outgoingData) override;

    void populateWebViewMenu(QMenu*, WebView*, const QWebHitTestResult&) override;

    Q_SLOT void openUrl(const QUrl&);

private:
    std::unique_ptr<Settings> m_settings;
    std::unique_ptr<AdBlockDash> m_adBlockDash;
    std::unique_ptr<SchemeHandler> m_schemeHandler;
    std::unique_ptr<QTranslator> m_translator;
};

} // namespace adblock_dash

#endif // ADBLOCK_DASH_PLUGIN_H
