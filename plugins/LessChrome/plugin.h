#ifndef LESSCHROMEPLUGIN_H
#define LESSCHROMEPLUGIN_H

#include "plugininterface.h"

#include "lc_settings.h"
#include "windowhandler.h"

#include <boost/scoped_ptr.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>

#include <QtCore/QObject>

class BrowserWindow;
class QMouseEvent;
class QString;
class QWidget;

namespace lesschrome {

//TODO Disable plugin by itself when unrecoverable error occur.
class Plugin
    : public QObject, public PluginInterface
{
    // This class is the DLL/Shared Library boundary.
    // Exceptions must not leak from this class.
    Q_OBJECT
    Q_INTERFACES(PluginInterface)

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "QupZilla.Browser.plugin.LessChrome")
#endif

public:
    Plugin();
    virtual ~Plugin();

    static Settings &settings();

private:
    // @override PluginInterface
    virtual PluginSpec pluginSpec();
    virtual void init(InitState state, const QString &settingsPath);
    virtual void unload();
    virtual bool testPlugin();
    virtual QTranslator* getTranslator(const QString &locale);
    virtual void showSettings(QWidget* parent = 0);

private slots:
    void mainWindowCreated(BrowserWindow *);
    void mainWindowDeleted(BrowserWindow *);

private:
    typedef boost::unordered_map<
        BrowserWindow*, boost::shared_ptr<WindowHandler> > HandlerMap;

    static boost::scoped_ptr<Settings> m_settings;

    HandlerMap m_windows;
    bool m_tabsOnTop;
};

} // namespace lesschrome

#endif // LESSCHROMEPLUGIN_H
