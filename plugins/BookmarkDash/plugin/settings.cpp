#include "settings.h"

namespace bookmark_dash {

Settings::
Settings(const QString &filename)
    : m_settings { filename, QSettings::IniFormat },
      m_bookmarksMenu { m_settings, "bookmarksMenu", true },
      m_bookmarksBar { m_settings, "bookmarksBar", true }
{
    m_settings.beginGroup("BookmarkDash");
}

} // namespace bookmark_dash

namespace bookmark_dash {

BoolSetting::
BoolSetting(QSettings &settings, const char key[], const bool defaultValue)
    : m_settings { settings },
      m_key { key },
      m_default { defaultValue }
{}

bool BoolSetting::
value() const
{
    const auto &value = m_settings.value(m_key, m_default);
    assert(value.template canConvert<bool>());
    return value.toBool();
}

void BoolSetting::
setValue(const bool newValue)
{
    if (value() != newValue) {
        m_settings.setValue(m_key, newValue);
        Q_EMIT changed(newValue);
    }
}

} // namespace bookmark_dash
