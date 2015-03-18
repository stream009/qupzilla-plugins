#ifndef BOOKMARK_DASH_SETTINGS_H
#define BOOKMARK_DASH_SETTINGS_H

#include <QtCore/QSettings>

namespace bookmark_dash {

class BoolSetting : public QObject
{
    Q_OBJECT
public:
    BoolSetting(QSettings &, const char key[], const bool defaultValue);

    bool value() const;

public Q_SLOTS:
    void setValue(const bool value);

Q_SIGNALS:
    void changed(bool);

private:
    QSettings &m_settings;
    const char* const m_key;
    bool m_default;
};

class Settings
{
public:
    explicit Settings(const QString &filename);

    BoolSetting &bookmarksMenu() { return m_bookmarksMenu; }
    BoolSetting &bookmarksBar() { return m_bookmarksBar; }

private:
    QSettings m_settings;
    BoolSetting m_bookmarksMenu;
    BoolSetting m_bookmarksBar;
};

} // namespace bookmark_dash

#endif // BOOKMARK_DASH_SETTINGS_H
