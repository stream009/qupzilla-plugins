#include "plugin.h"

#include "error.h"
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
try : QObject(),
      m_tabsOnTop(::qzSettings->tabsOnTop)
{}
catch (const std::exception &e) {
    defaultExceptionHandler(__FUNCTION__, e);
}

Plugin::
~Plugin()
try {
    ::qzSettings->tabsOnTop = m_tabsOnTop;
}
catch (const std::exception &e) {
    defaultExceptionHandler(__FUNCTION__, e);
}

Settings &Plugin::
settings()
try {
    assert(m_settings);
    if (!m_settings) {
        throw RuntimeError("Setting isn't initialized.");
    }
    return *m_settings;
}
catch (const std::exception &e) {
    defaultExceptionHandler(__FUNCTION__, e);
}

PluginSpec Plugin::
pluginSpec()
try {
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
catch (const std::exception &e) {
    defaultExceptionHandler(__FUNCTION__, e);
}

void Plugin::
init(InitState state, const QString &settingsPath)
try {
    //qDebug() << __FUNCTION__;
    assert(mApp->plugins());

    if (!m_settings) {
        m_settings.reset(
            new Settings(settingsPath + QL1S("/extensions.ini")));
    }

    if (!mApp->plugins()) {
        throw RuntimeError("Fail to obtain plugin delegate");
    }
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
catch (const std::exception &e) {
    defaultExceptionHandler(__FUNCTION__, e);
}

void Plugin::
unload()
try {
    //qDebug() << __FUNCTION__;
}
catch (const std::exception &e) {
    defaultExceptionHandler(__FUNCTION__, e);
}

bool Plugin::
testPlugin()
try {
    return (Qz::VERSION == QL1S(QUPZILLA_VERSION));
}
catch (const std::exception &e) {
    defaultExceptionHandler(__FUNCTION__, e);
}

QTranslator* Plugin::
getTranslator(const QString &locale)
try {
    QTranslator* const translator = new QTranslator(this);
    translator->load(locale, ":/lesschrome/locale/");
    return translator;
}
catch (const std::exception &e) {
    defaultExceptionHandler(__FUNCTION__, e);
}

void Plugin::
showSettings(QWidget* const parent)
try {
    assert(parent);
    if (!parent) {
        throw RuntimeError("invalid window");
    }

    SettingDialog dialog(*parent);
    dialog.exec();
}
catch (const std::exception &e) {
    defaultExceptionHandler(__FUNCTION__, e);
}

void Plugin::
mainWindowCreated(BrowserWindow* const window)
try {
    //qDebug() << __FUNCTION__ << window;
    assert(window);

    if (!window) {
        throw RuntimeError("invalid window");
    }

    boost::shared_ptr<WindowHandler> ptr
            = boost::make_shared<WindowHandler, BrowserWindow&>(*window);
    m_windows.emplace(window, ptr);
}
catch (const std::exception &e) {
    defaultExceptionHandler(__FUNCTION__, e);
}

void Plugin::
mainWindowDeleted(BrowserWindow* const window)
try {
    //qDebug() << __FUNCTION__;
    assert(window);

    if (!window) {
        throw RuntimeError("invalid window");
    }
    m_windows.erase(window);
}
catch (const std::exception &e) {
    defaultExceptionHandler(__FUNCTION__, e);
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(LessChrome, Plugin)
#endif

} // namespace lesschrome
