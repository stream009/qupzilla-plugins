#include "plugin.h"

#include "browserwindow.h"
#include "pluginproxy.h"
#include "qzcommon.h"
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
    : QObject()
{}

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
    qDebug() << __FUNCTION__ << "called";
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
    //qDebug() << obj << event->pos();

    if (type != Qz::ON_WebView) return false;
    BrowserWindow * const window =
                    qobject_cast<TabbedWebView*>(obj)->browserWindow();
    if (window == NULL) return false;

    boost::shared_ptr<WindowHandler> handler = m_windows.at(window);
    handler->mouseMove(event);

    return false;
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(LessChrome, LessChromePlugin)
#endif

} // namespace lesschrome
