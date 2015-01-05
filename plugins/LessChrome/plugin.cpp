#include "plugin.h"

#include "browserwindow.h"
#include "pluginproxy.h"
#include "qzcommon.h"
#include "qzsettings.h"
#include "tabbedwebview.h"

#include <cassert>

#include <boost/make_shared.hpp>

#include <QtCore/QTranslator>
#include <QtGui/QDialog>
#include <QtGui/QLabel>
#include <QtGui/QMouseEvent>
#include <QtGui/QPushButton>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

namespace lesschrome {

LessChromePlugin::
LessChromePlugin()
    : QObject(),
      m_tabsOnTop(qzSettings->tabsOnTop)
{}

LessChromePlugin::
~LessChromePlugin()
{
    qzSettings->tabsOnTop = m_tabsOnTop;
}

PluginSpec LessChromePlugin::
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

void LessChromePlugin::
init(InitState state, const QString &settingsPath)
{
    qDebug() << __FUNCTION__ << "called";

    m_settingsPath = settingsPath;

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
}

void LessChromePlugin::
unload()
{
    qDebug() << __FUNCTION__;
}

bool LessChromePlugin::
testPlugin()
{
    return (Qz::VERSION == QLatin1String(QUPZILLA_VERSION));
}

QTranslator* LessChromePlugin::
getTranslator(const QString &locale)
{
    // Loads translation according to locale file
    // QString locale will contains "fr_FR.qm" for French locale

    QTranslator* translator = new QTranslator(this);
    translator->load(locale, ":/lesschrome/locale/");
    return translator;
}

void LessChromePlugin::
showSettings(QWidget* parent)
{
    QDialog settings(parent);
    QPushButton* b = new QPushButton("LessChrome v0.0.1");
    QPushButton* closeButton = new QPushButton(tr("Close"));
    QLabel* label = new QLabel();
    label->setPixmap(QPixmap(":icons/other/about.png"));

    QVBoxLayout* l = new QVBoxLayout(&settings);
    l->addWidget(label);
    l->addWidget(b);
    l->addWidget(closeButton);
    settings.setLayout(l);

    settings.setWindowTitle(tr("LessChrome Plugin Settings"));
    settings.setWindowIcon(QIcon(":qupzilla.png"));
    connect(closeButton, SIGNAL(clicked()), &settings, SLOT(close()));

    settings.exec();
}

void LessChromePlugin::
mainWindowCreated(BrowserWindow *window)
{
    qDebug() << __FUNCTION__ << window;
    m_windows.emplace(window,
                      boost::make_shared<WindowHandler>(window));
}

void LessChromePlugin::
mainWindowDeleted(BrowserWindow * const window)
{
    qDebug() << __FUNCTION__;
    m_windows.erase(window);
}

bool LessChromePlugin::
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

    //TODO check
    boost::shared_ptr<WindowHandler> handler = m_windows.at(window);

    // Translate event in BrowserWindow's coordinate system.
    const QPoint pos = webView->mapTo(window, event->pos());
    QMouseEvent ev(event->type(), pos, event->button(),
                                 event->buttons(), event->modifiers());
    // could be passing only pos if that is enough.
    handler->mouseMove(&ev);

    return false;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(LessChrome, LessChromePlugin)
#endif

} // namespace lesschrome
