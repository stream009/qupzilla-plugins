#include "plugin.h"

#include "webpage.h"
#include "styles.h"
#include "utility.h"
#include "gui/settingdialog.h"
#include "util/error.h"
#include "serialization/styles.h"

#include <pluginproxy.h>
#include <../webkit/webpage.h>

#include <cassert>

#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/make_unique.hpp>
#include <boost/serialization/unique_ptr.hpp>

#include <QtCore/QTranslator>
#include <QtGui/QWidget>

namespace stylist {

std::unique_ptr<Settings> Plugin::m_settings;
std::unique_ptr<Styles> Plugin::m_styles;

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

Styles &Plugin::
styles() noexcept
{
    assert(m_styles);
    return *m_styles;
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

        namespace bfs = boost::filesystem;
        m_pluginPath = settingsPath.toLocal8Bit().constData();
        m_pluginPath /= "stylist";
        if (!bfs::exists(m_pluginPath)) {
            if (!bfs::create_directory(m_pluginPath)) {
                assert(false); //TODO better
            }
        }

        assert(!m_settings);
        m_settings.reset(
            new Settings(settingsPath + QL1S("/extensions.ini")));

        assert(!m_styles);

        const Path &dataPath = m_pluginPath / "styles.dat"; //TODO factor out
        if (bfs::exists(dataPath)) {
            bfs::ifstream ifs { dataPath };
            assert(ifs.good()); //TODO better
            boost::archive::text_iarchive dat { ifs };

            dat >> m_styles; //TODO handle exception
        }
        else {
            m_styles.reset(new Styles { m_pluginPath });
            m_styles->scanDirectory();
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

        using WebPage = ::WebPage;
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
        namespace bfs = boost::filesystem;

        const Path &dataPath = m_pluginPath / "styles.dat"; //TODO factor out
        bfs::ofstream ofs { dataPath };
        assert(ofs.good()); //TODO better
        boost::archive::text_oarchive dat { ofs };

        dat << m_styles; //TODO handle exception
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

        SettingDialog dialog(*parent);
        dialog.exec();
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
    qDebug() << __FUNCTION__;
    assert(webPage);
    try {
        if (!webPage) {
            throw RuntimeError("Receive invalid Web page.");
        }

        this->connect(webPage, SIGNAL(destroyed()),
                      this,    SLOT(slotWebPageDestroyed()));

        m_webPages.emplace(webPage, boost::make_unique<Page>(webPage));
    }
    catch (const std::exception &e) {
        DEFAULT_EXCEPTION_HANDLER(e);
    }
}

void Plugin::
slotWebPageDestroyed()
{
    auto* const webPage = this->sender();
    qDebug() << __FUNCTION__ << webPage;

    assert(m_webPages.count(webPage) == 1);
    m_webPages.erase(webPage);
}

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(Stylist, Plugin)
#endif

} // namespace stylist
