#include "plugin.h"

#include "error.h"

#include <pluginproxy.h>

#include <cassert>

#include <QtCore/QTranslator>
#include <QtGui/QWidget>

namespace stylist {

//boost::scoped_ptr<Settings> Plugin::m_settings;

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

PluginSpec Plugin::
pluginSpec() // noexcept
{
    PluginSpec spec;
    try {
        spec.name = "Stylist";
        spec.info = "Load user style sheet automatically";
        spec.description = "Load user style sheet automatically";
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
            this,            SLOT(slotMainWindowCreated(BrowserWindow*)));
        this->connect(
            mApp->plugins(), SIGNAL(mainWindowDeleted(BrowserWindow*)),
            this,            SLOT(slotMainWindowDeleted(BrowserWindow*)));
        this->connect(
            mApp->plugins(), SIGNAL(webPageCreated(WebPage*)),
            this,            SLOT(slotWebPageCreated(WebPage*)));

        if (state == LateInitState) {
            for (BrowserWindow* const window: mApp->windows()) {
                slotMainWindowCreated(window);
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
    //qDebug() << __FUNCTION__;
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
        translator->load(locale, ":/stylist/locale/");
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

        //SettingDialog dialog(*parent);
        //dialog.exec();
    }
    catch (const std::exception &e) {
        DEFAULT_EXCEPTION_HANDLER(e);
    }
}

void Plugin::
slotMainWindowCreated(BrowserWindow* const window) noexcept
{
    //qDebug() << __FUNCTION__ << window;
    assert(window);
    try {
        if (!window) {
            throw RuntimeError("invalid window");
        }
    }
    catch (const std::exception &e) {
        DEFAULT_EXCEPTION_HANDLER(e);
    }
}

void Plugin::
slotMainWindowDeleted(BrowserWindow* const window) noexcept
{
    //qDebug() << __FUNCTION__;
    assert(window);
    try {
        if (!window) {
            throw RuntimeError("invalid window");
        }
    }
    catch (const std::exception &e) {
        DEFAULT_EXCEPTION_HANDLER(e);
    }
}

void Plugin::
slotWebPageCreated(WebPage* const webPage) noexcept
{
    //qDebug() << __FUNCTION__;
    assert(webPage);
    try {
        if (!webPage) {
            throw RuntimeError("Receive invalid Web page.");
        }
    }
    catch (const std::exception &e) {
        DEFAULT_EXCEPTION_HANDLER(e);
    }
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Stylist, Plugin)
#endif

} // namespace stylist