#include "settings.hpp"

#include "common/utility.hpp"

#include <boost/filesystem.hpp>
#include <boost/make_unique.hpp>

namespace adblock_dash {

Settings::
Settings(const QString &settingsPath)
    : m_settings {
        settingsPath + "/extensions.ini", QSettings::IniFormat }
{
    m_settings.beginGroup("AdBlockDash");

    m_directory = boost::make_unique<StringSetting>(
        m_settings, "directory", settingsPath + "/adblock_dash"
    );

    m_subscriptions.reset(
                new Subscriptions { m_settings, "Subscriptions" });
    assert(m_subscriptions);

    m_customLists.reset(new FilterSets { m_settings, "CustomList" });
    assert(m_customLists);

    m_acceptableAdsUrl = boost::make_unique<StringSetting>(
        m_settings,
        "AcceptableAdsUrl",
        "https://adblockplus.org/en/acceptable-ads"
    );
    assert(m_acceptableAdsUrl);

    m_newFilterSetName = boost::make_unique<StringSetting>(
        m_settings,
        "NewFilterSetName",
        "New Filter Set"
    );
    assert(m_newFilterSetName);

    m_commentColor = boost::make_unique<ColorSetting>(
        m_settings,
        "CommentColor",
        "gray"
    );
    assert(m_commentColor);

    m_elementHidingRuleColor = boost::make_unique<ColorSetting>(
        m_settings,
        "ElementHidingRuleColor",
        "blue"
    );
    assert(m_elementHidingRuleColor);

    m_exceptionRuleColor = boost::make_unique<ColorSetting>(
        m_settings,
        "ExceptionRuleColor",
        "green"
    );
    assert(m_exceptionRuleColor);

    m_knownSubscriptionsUrl = boost::make_unique<StringSetting>(
        m_settings,
        "KnownSubscriptionsUrl",
        "https://adblockplus.org/subscriptions"
    );

    loadExceptionRules();

    namespace bfs = boost::filesystem;
    const auto &utf8Path = m_directory->value().toLocal8Bit();
    if (!bfs::exists(utf8Path.constData())) {
        if (!bfs::create_directory(utf8Path.constData())) {
            assert(false); //TODO better
        }
    }

    validate();
}

Settings::
~Settings()
{
    saveExceptionRules();
}

void Settings::
loadExceptionRules()
{
    Path path;
    const auto &pathStr = m_settings.value("ExceptionRulesPath").toString();
    if (!pathStr.isNull()) {
        path = pathStr.toLocal8Bit().constData();
    }
    else {
        path = m_directory->value().toLocal8Bit().constData();
        path /= "exceptionrules.txt";
    }

    const auto &location = m_settings.value(
        "ExceptionRulesUrl",
        "https://easylist-downloads.adblockplus.org/exceptionrules.txt"
    ).toString();
    assert(!location.isNull());

    const auto enabled =
            m_settings.value("ExceptionRulesEnabled", false).toBool();

    m_exceptionRules = boost::make_unique<Subscription>(
            "Allow non-intrusive advertising", path, location, enabled);

}

void Settings::
saveExceptionRules()
{
    m_settings.setValue(
        "ExceptionRulesPath", QString { m_exceptionRules->path().c_str() });
    m_settings.setValue(
        "ExceptionRulesUrl", m_exceptionRules->location().toString());
    m_settings.setValue(
        "ExceptionRulesEnabled", m_exceptionRules->enabled());
}

} // namespace adblock_dash
