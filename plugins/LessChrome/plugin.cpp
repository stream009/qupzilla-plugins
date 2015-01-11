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
    : QObject(),
      m_tabsOnTop(::qzSettings->tabsOnTop)
{
    try {
    }
    catch (const std::exception &e) {
        DEFAULT_EXCEPTION_HANDLER(e);
    }
}

Plugin::
~Plugin()
{
    try {
        ::qzSettings->tabsOnTop = m_tabsOnTop;
    }
    catch (const std::exception &e) {
        DEFAULT_EXCEPTION_HANDLER(e);
    }
}

Settings &Plugin::
settings()
{
    assert(m_settings);
    try {
    }
    catch (const std::exception &e) {
        DEFAULT_EXCEPTION_HANDLER(e);
    }
    return *m_settings;
}

PluginSpec Plugin::
pluginSpec()
{
    PluginSpec spec;
    try {
        spec.name = "LessChrome";
        spec.info = "Plugin that widen browsing area";
        spec.description = "Hide location bar and bookmark bar when "
                           "they are not necessary.";
        spec.version = "0.0.1";
        spec.author = "stream9 <stream009@gmail.com>";
        spec.icon = QPixmap(":qupzilla.png");
        spec.hasSettings = true;
    }
    catch (const std::exception &e) {
        DEFAULT_EXCEPTION_HANDLER(e);
    }
    return spec;
}

void Plugin::
init(InitState state, const QString &settingsPath)
{
    //qDebug() << __FUNCTION__;
    try {
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
    }
    catch (const std::exception &e) {
        DEFAULT_EXCEPTION_HANDLER(e);
    }

    assert(m_settings);
}

void Plugin::
unload()
{
    //qDebug() << __FUNCTION__;
    try {
    }
    catch (const std::exception &e) {
        DEFAULT_EXCEPTION_HANDLER(e);
    }
}

bool Plugin::
testPlugin()
{
    try {
        return (Qz::VERSION == QL1S(QUPZILLA_VERSION));
    }
    catch (const std::exception &e) {
        DEFAULT_EXCEPTION_HANDLER(e);
        return false;
    }
}

QTranslator* Plugin::
getTranslator(const QString &locale)
{
    QTranslator* const translator = new QTranslator(this);
    try {
        translator->load(locale, ":/lesschrome/locale/");
    }
    catch (const std::exception &e) {
        DEFAULT_EXCEPTION_HANDLER(e);
    }
    return translator;
}

void Plugin::
showSettings(QWidget* const parent)
{
    assert(parent);
    try {
        if (!parent) {
            throw RuntimeError("invalid window");
        }

        SettingDialog dialog(*parent);
        dialog.exec();
    }
    catch (const std::exception &e) {
        DEFAULT_EXCEPTION_HANDLER(e);
    }
}

void Plugin::
mainWindowCreated(BrowserWindow* const window)
{
    //qDebug() << __FUNCTION__ << window;
    assert(window);
    try {
        if (!window) {
            throw RuntimeError("invalid window");
        }

        boost::shared_ptr<WindowHandler> ptr
                = boost::make_shared<WindowHandler, BrowserWindow&>(*window);
        m_windows.emplace(window, ptr);
    }
    catch (const std::exception &e) {
        DEFAULT_EXCEPTION_HANDLER(e);
    }
}

void Plugin::
mainWindowDeleted(BrowserWindow* const window)
{
    //qDebug() << __FUNCTION__;
    assert(window);
    try {
        if (!window) {
            throw RuntimeError("invalid window");
        }
        m_windows.erase(window);
    }
    catch (const std::exception &e) {
        DEFAULT_EXCEPTION_HANDLER(e);
    }
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(LessChrome, Plugin)
#endif

} // namespace lesschrome
