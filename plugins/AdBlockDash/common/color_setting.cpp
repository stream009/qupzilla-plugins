#include "color_setting.hpp"

#include <cassert>

#include <QtCore/QSettings>
#include <QtCore/QString>

ColorSetting::
ColorSetting(QSettings &settings,
              const QString &key, const QColor &defaultValue)
    : m_settings { settings },
      m_key { key },
      m_defaultColorName { defaultValue.name() }
{}

QColor ColorSetting::
value() const
{
    const auto &value = m_settings.value(m_key, m_defaultColorName);
    assert(value.template canConvert<QString>());

    const QColor result { value.toString() };
    assert(result.isValid());

    return result;
}

void ColorSetting::
setValue(const QColor &newValue)
{
    assert(newValue.isValid());
    if (value() == newValue) return;

    m_settings.setValue(m_key, newValue.name());
    Q_EMIT changed(newValue);
}
