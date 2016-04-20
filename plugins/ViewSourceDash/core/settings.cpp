#include "settings.hpp"

#include <cassert>

#include <QtCore/QDir>

#include <settings.h>

namespace view_source_dash {

Settings::
Settings(const QString &path)
    : m_settings { path + "/extensions.ini", QSettings::IniFormat }
{
    m_settings.beginGroup("ViewSourceDash");
}

QString Settings::
viewer() const
{
    return m_settings.value("viewer", "/usr/bin/gvim").toString();
}

void Settings::
setViewer(const QString &viewer)
{
    m_settings.setValue("viewer", viewer);
}

static QDir
getPluginDir()
{
    ::Settings qupzillaSettings;

    qupzillaSettings.beginGroup("Plugin-Settings");

    const auto &allowedPlugins = qupzillaSettings.value(
                          "AllowedPlugins", QStringList {}).toStringList();

    qupzillaSettings.endGroup();

    const auto it = std::find_if(allowedPlugins.begin(), allowedPlugins.end(),
        [] (const QString &path) {
            return path.contains("ViewSourceDash");
        });
    assert(it != allowedPlugins.end());

    const auto &path = *it;
    const auto idx = path.lastIndexOf("/");
    assert(idx != -1);

    return { path.left(idx) };
}

QString Settings::
dialogPluginPath() const
{
    auto &&pluginDir = getPluginDir();

    QStringList nameFilters;
    nameFilters << "*ViewSourceDashSetting*";
    pluginDir.setNameFilters(nameFilters);

    const auto &entries = pluginDir.entryList();
    assert(entries.size() == 1);

    return pluginDir.absolutePath() + "/" + entries.front();
}

} // namespace view_source_dash
