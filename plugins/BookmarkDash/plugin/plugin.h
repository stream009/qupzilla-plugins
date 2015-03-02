#ifndef BOOKMARK_DASH_PLUGIN_H
#define BOOKMARK_DASH_PLUGIN_H

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

namespace bookmark_dash {

class WindowAdaptor;

class Plugin : public QObject, public PluginInterface
{
    // This class is the DLL/Shared Library boundary.
    // Exceptions must not leak from this class.
    Q_OBJECT
    Q_INTERFACES(PluginInterface)

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "QupZilla.Browser.plugin.BookmarkDash")
#endif
    using Path = boost::filesystem::path;
    using WindowMap = std::unordered_map<
                    BrowserWindow*, std::unique_ptr<WindowAdaptor>>;
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
    void onMainWindowCreated(BrowserWindow*) noexcept;
    void onMainWindowDeleted(BrowserWindow*) noexcept;

private:
    static std::unique_ptr<Settings> m_settings;
    static Path m_pluginPath;

    WindowMap m_windows;
};

} // namespace bookmark_dash

#endif // BOOKMARK_DASH_PLUGIN_H
