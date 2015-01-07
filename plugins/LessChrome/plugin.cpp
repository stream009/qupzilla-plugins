#include "plugin.h"

#include "settingdialog.h"

#include "browserwindow.h"
#include "pluginproxy.h"
#include "qzcommon.h"
#include "qzsettings.h"
#include "tabbedwebview.h"

#include <cassert>

#include <boost/make_shared.hpp>

#include <QtCore/QTranslator>
#include <QtGui/QDialog>
#include <QtGui/QMouseEvent>
#include <QtGui/QWidget>

namespace lesschrome {

boost::scoped_ptr<Settings> Plugin::m_settings;

Plugin::
Plugin()
    : QObject(),
      m_tabsOnTop(::qzSettings->tabsOnTop)
{}

Plugin::
~Plugin()
{
    ::qzSettings->tabsOnTop = m_tabsOnTop;
}

Settings &Plugin::
settings()
{
    assert(m_settings);
    return *m_settings;
}

PluginSpec Plugin::
pluginSpec()
{
    PluginSpec spec;
    spec.name = "LessChrome";
    spec.info = "Plugin that widen browsing area";
    spec.description = "Hide location bar and bookmark bar when "
                       "they are not necessary.";
    spec.version = "0.0.1";
    spec.author = "stream9 <stream009@gmail.com>";
    spec.icon = QPixmap(":qupzilla.png");
    spec.hasSettings = true;

    return spec;
}

void Plugin::
init(InitState state, const QString &settingsPath)
{
    qDebug() << __FUNCTION__;

    if (!m_settings) {
        m_settings.reset(
            new Settings(settingsPath + QL1S("/extensions.ini")));
    }

    QZ_REGISTER_EVENT_HANDLER(PluginProxy::MouseMoveHandler);

    this->connect(
        mApp->plugins(), SIGNAL(mainWindowCreated(BrowserWindow*)),
        this,            SLOT(mainWindowCreated(BrowserWindow*)));
    this->connect(
        mApp->plugins(), SIGNAL(mainWindowDeleted(BrowserWindow*)),
        this,            SLOT(mainWindowDeleted(BrowserWindow*)));

    if (state == LateInitState) {
        foreach (BrowserWindow* const window, mApp->windows()) {
            mainWindowCreated(window);
        }
    }

    assert(m_settings);
}

void Plugin::
unload()
{
    qDebug() << __FUNCTION__;
}

bool Plugin::
testPlugin()
{
    return (Qz::VERSION == QL1S(QUPZILLA_VERSION));
}

QTranslator* Plugin::
getTranslator(const QString &locale)
{
    QTranslator* translator = new QTranslator(this);
    translator->load(locale, ":/lesschrome/locale/");
    return translator;
}

void Plugin::
showSettings(QWidget* parent)
{
    assert(m_settings);

    SettingDialog dialog(parent);
    dialog.exec();
}

void Plugin::
mainWindowCreated(BrowserWindow *window)
{
    qDebug() << __FUNCTION__ << window;
    assert(window);
    assert(m_settings);

    boost::shared_ptr<WindowHandler> ptr
                            = boost::make_shared<WindowHandler>(window);
    this->connect(m_settings.get(), SIGNAL(change(QString)),
                  ptr.get(),        SLOT(slotSettingChanged(const QString&)));

    m_windows.emplace(window, ptr);
}

void Plugin::
mainWindowDeleted(BrowserWindow * const window)
{
    qDebug() << __FUNCTION__;
    m_windows.erase(window);
}

bool Plugin::
mouseMove(const Qz::ObjectName &type, QObject * const obj,
                                      QMouseEvent * const event)
{
    assert(obj);
    assert(event);
    //qDebug() << obj << event->pos();

    if (type != Qz::ON_WebView) return false;

    TabbedWebView* const webView = qobject_cast<TabbedWebView*>(obj);
    assert(webView); //TODO better handling

    BrowserWindow * const window = webView->browserWindow();
    if (window == NULL) return false;

    //TODO std::out_of_range
    boost::shared_ptr<WindowHandler> handler = m_windows.at(window);

    // Translate event into BrowserWindow's coordinate system.
    const QPoint pos = webView->mapTo(window, event->pos());
    QMouseEvent ev(event->type(), pos, event->button(),
                                 event->buttons(), event->modifiers());
    // could be passing only pos if that is enough.
    handler->mouseMove(&ev);

    return false;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(LessChrome, Plugin)
#endif

} // namespace lesschrome
