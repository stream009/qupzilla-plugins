#ifndef STYLIST_PLUGIN_H
#define STYLIST_PLUGIN_H

#include "plugininterface.h"

#include "settings.h"

#include <memory>

#include <QtCore/QObject>

class BrowserWindow;
class QString;
class QWidget;
class WebPage;

namespace stylist {

class Plugin
    : public QObject, public PluginInterface
{
    // This class is the DLL/Shared Library boundary.
    // Exceptions must not leak from this class.
    Q_OBJECT
    Q_INTERFACES(PluginInterface)

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "QupZilla.Browser.plugin.Stylist")
#endif

public:
    Plugin() noexcept;
    virtual ~Plugin() noexcept;

    static Settings &settings() noexcept;

private:
    // @override PluginInterface. noexcept
    virtual PluginSpec pluginSpec();
    virtual void init(InitState state, const QString &settingsPath);
    virtual void unload();
    virtual bool testPlugin();
    virtual QTranslator* getTranslator(const QString &locale);
    virtual void showSettings(QWidget* parent = 0);

private slots:
    void slotMainWindowCreated(BrowserWindow* const) noexcept;
    void slotMainWindowDeleted(BrowserWindow* const) noexcept;
    void slotWebPageCreated(WebPage* const) noexcept;

private:
    static std::unique_ptr<Settings> m_settings;
};

} // namespace stylist

#endif // STYLIST_PLUGIN_H
