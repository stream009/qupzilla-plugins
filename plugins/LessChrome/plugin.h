#ifndef LESSCHROMEPLUGIN_H
#define LESSCHROMEPLUGIN_H

#include "plugininterface.h"

#include "windowhandler.h"

#include <boost/shared_ptr.hpp>
#include <boost/unordered_map.hpp>

#include <QtCore/QObject>
#include <QtCore/QString>

class BrowserWindow;
class QMouseEvent;
class QWidget;

namespace lesschrome {

class LessChromePlugin
    : public QObject, public PluginInterface
{
    Q_OBJECT
    Q_INTERFACES(PluginInterface)

#if QT_VERSION >= 0x050000
    Q_PLUGIN_METADATA(IID "QupZilla.Browser.plugin.LessChrome")
#endif

public:
    LessChromePlugin();
    virtual ~LessChromePlugin();

    // @override PluginInterface
    virtual PluginSpec pluginSpec();

    virtual void init(InitState state, const QString &settingsPath);
    virtual void unload();
    virtual bool testPlugin();

    virtual QTranslator* getTranslator(const QString &locale);
    virtual void showSettings(QWidget* parent = 0);

    virtual bool mouseMove(const Qz::ObjectName &type,
                           QObject* obj, QMouseEvent* event);

private slots:
    void mainWindowCreated(BrowserWindow *);
    void mainWindowDeleted(BrowserWindow *);

private:
    typedef boost::unordered_map<
        BrowserWindow*, boost::shared_ptr<WindowHandler> > HandlerMap;

    QString m_settingsPath;
    HandlerMap m_windows;
    bool m_tabsOnTop;
};

} // namespace lesschrome

#endif // LESSCHROMEPLUGIN_H
