#include "setting.hpp"

#include <cassert>
#include <QtCore/QSettings>

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

#include <cassert>
#include <QtCore/QSettings>
#include <QtCore/QString>

StringSetting::
StringSetting(QSettings &settings,
              const QString &key, const QString &defaultValue)
    : m_settings { settings },
      m_key { key },
      m_defaultValue { defaultValue }
{}

QString StringSetting::
value() const
{
    const auto &value = m_settings.value(m_key, m_defaultValue);
    assert(value.template canConvert<QString>());
    return value.toString();
}

void StringSetting::
setValue(const QString &newValue)
{
    if (value() != newValue) {
        m_settings.setValue(m_key, newValue);
        Q_EMIT changed(newValue);
    }
}
