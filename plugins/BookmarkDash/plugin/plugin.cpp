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
#include <QtGui/QMouseEvent>

namespace bookmark_dash {

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
settings() const
{
    assert(m_settings);
    return *m_settings;
}

Qt::MouseButtons Plugin::
recentlyPressedButtons() const
{
    return m_recentlyPressedButtons;
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
        spec.hasSettings = false;
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
        assert(mApp);
        mApp->installEventFilter(this);
        assert(mApp->plugins());

        m_settings.reset(
            new Settings(settingsPath + QL1S("/extensions.ini")));
        assert(m_settings);

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

bool Plugin::
eventFilter(QObject* const, QEvent* const ev)
{
    assert(ev);
    if (ev->type() == QEvent::MouseButtonPress) {
        auto &event = static_cast<QMouseEvent&>(*ev);
        m_recentlyPressedButtons = event.buttons();
    }

    return false;
}

void Plugin::
onMainWindowCreated(BrowserWindow* const window) noexcept
{
    //qDebug() << __func__ << window;
    assert(window);
    try {
        m_windows.emplace(
            window, boost::make_unique<WindowAdaptor>(*window, *this));
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
