#include "plugin.h"

#include "windowadaptor.h"

#include "common/utility.h"
#include "common/error.h"

#include <pluginproxy.h>
#include <webpage.h>

#include <cassert>

#include <boost/make_unique.hpp>

#include <QtCore/QTranslator>
#include <QtGui/QWidget>

namespace bookmark_dash {

std::unique_ptr<Settings> Plugin::m_settings;
Plugin::Path Plugin::m_pluginPath;

Plugin::
Plugin() noexcept
{
    try {
    }
    catch (const std::exception &e) {
        DEFAULT_EXCEPTION_HANDLER(e);
    }
}

Plugin::
~Plugin() noexcept
{
    try {
    }
    catch (const std::exception &e) {
        DEFAULT_EXCEPTION_HANDLER(e);
    }
}

Settings &Plugin::
settings() noexcept
{
    assert(m_settings);
    return *m_settings;
}

const Plugin::Path &Plugin::
directory() noexcept
{
    assert(!m_pluginPath.empty());
    return m_pluginPath;
}

PluginSpec Plugin::
pluginSpec() // noexcept
{
    PluginSpec spec;
    try {
        spec.name = "Bookmark Dash";
        spec.info = "Enhance bookmark functionality a bit.";
        spec.description = "";
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
init(InitState state, const QString &settingsPath) // noexcept
{
    //qDebug() << __func__;
    try {
        assert(mApp->plugins());

        namespace bfs = boost::filesystem;
        m_pluginPath = settingsPath.toLocal8Bit().constData();
        m_pluginPath /= "bookmark_dash";
        if (!bfs::exists(m_pluginPath)) {
            if (!bfs::create_directory(m_pluginPath)) {
                assert(false); //TODO better
            }
        }

        assert(!m_settings);
        m_settings.reset(
            new Settings(settingsPath + QL1S("/extensions.ini")));

        if (!mApp->plugins()) {
            throw RuntimeError("Fail to obtain plugin delegate");
        }
        this->connect(
            mApp->plugins(), SIGNAL(mainWindowCreated(BrowserWindow*)),
            this,            SLOT(onMainWindowCreated(BrowserWindow*)));
        this->connect(
            mApp->plugins(), SIGNAL(mainWindowDeleted(BrowserWindow*)),
            this,            SLOT(onMainWindowDeleted(BrowserWindow*)));

        if (state == LateInitState) {
            for (BrowserWindow* const window: mApp->windows()) {
                onMainWindowCreated(window);
            }
        }
    }
    catch (const std::exception &e) {
        DEFAULT_EXCEPTION_HANDLER(e);
    }

    assert(m_settings);
}

void Plugin::
unload() // noexcept
{
    //qDebug() << __func__;
    try {
    }
    catch (const std::exception &e) {
        DEFAULT_EXCEPTION_HANDLER(e);
    }
}

bool Plugin::
testPlugin() // noexcept
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
getTranslator(const QString &locale) // noexcept
{
    QTranslator* const translator = new QTranslator(this);
    try {
        translator->load(locale, ":/bookmark_dash/locale/");
    }
    catch (const std::exception &e) {
        DEFAULT_EXCEPTION_HANDLER(e);
    }
    return translator;
}

void Plugin::
showSettings(QWidget* const parent) // noexcept
{
    assert(parent);
    try {
        if (!parent) {
            throw RuntimeError("invalid window");
        }
    }
    catch (const std::exception &e) {
        DEFAULT_EXCEPTION_HANDLER(e);
    }
}

void Plugin::
onMainWindowCreated(BrowserWindow* const window) noexcept
{
    //qDebug() << __func__ << window;
    assert(window);
    try {
        if (!window) {
            throw RuntimeError("invalid window");
        }
        m_windows.emplace(
            window, boost::make_unique<WindowAdaptor>(*window));
    }
    catch (const std::exception &e) {
        DEFAULT_EXCEPTION_HANDLER(e);
    }
}

void Plugin::
onMainWindowDeleted(BrowserWindow* const window) noexcept
{
    //qDebug() << __func__;
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
Q_EXPORT_PLUGIN2(BookmarkDash, Plugin)
#endif

} // namespace bookmark_dash
