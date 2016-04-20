#ifndef VIEW_SOURCE_DASH_SETTINGS_HPP
#define VIEW_SOURCE_DASH_SETTINGS_HPP

#include <QtCore/QSettings>

class QString;

namespace view_source_dash {

class Settings
{
public:
    Settings(const QString &path);

    QString viewer() const;
    void setViewer(const QString&);

    QString dialogPluginPath() const;

private:
    QSettings m_settings;
};

} // namespace view_source_dash

#endif // VIEW_SOURCE_DASH_SETTINGS_HPP
