#ifndef STYLIST_PLUGIN_H
#define STYLIST_PLUGIN_H

#include "settings.h"

#include <plugininterface.h>

#include <memory>
#include <unordered_map>

#include <boost/filesystem.hpp>

#include <QtCore/QObject>

class BrowserWindow;
class QString;
class QWidget;
class WebPage;

namespace stylist {

class Page;
class Styles;

class Plugin : public QObject, public PluginInterface
{
    // This class is the DLL/Shared Library boundary.
    // Exceptions must not leak from this class.
    Q_OBJECT
    Q_INTERFACES(PluginInterface)

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "QupZilla.Browser.plugin.Stylist")
#endif
    using Path = boost::filesystem::path;

public:
    Plugin() noexcept;
    virtual ~Plugin() noexcept;

    static Settings &settings() noexcept;
    static const Path &directory() noexcept;

private:
    // @override PluginInterface. noexcept
    PluginSpec pluginSpec() override;
    void init(InitState state, const QString &settingsPath) override;
    void unload() override;
    bool testPlugin() override;
    QTranslator* getTranslator(const QString &locale) override;
    void showSettings(QWidget* parent = 0) override;

private Q_SLOTS:
    void slotMainWindowCreated(BrowserWindow*) noexcept;
    void slotMainWindowDeleted(BrowserWindow*) noexcept;
    void slotWebPageCreated(WebPage*) noexcept;
    void slotWebPageDestroyed();

private:
    static std::unique_ptr<Settings> m_settings;
    static Path m_pluginPath;

    std::unordered_map<QObject*, std::unique_ptr<Page>> m_webPages;
};

} // namespace stylist

#endif // STYLIST_PLUGIN_H
