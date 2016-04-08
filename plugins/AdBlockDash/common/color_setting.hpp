#ifndef UTILITY_COLOR_SETTING_HPP
#define UTILITY_COLOR_SETTING_HPP

#include <QtCore/QObject>
#include <QtGui/QColor>

class QColor;
class QSettings;
class QString;

class ColorSetting : public QObject
{
    Q_OBJECT
public:
    // constructor
    ColorSetting(QSettings&,
           const QString &key, const QColor &defaultValue);

    // operator
    ColorSetting &operator=(const QColor &value)
    {
        setValue(value);
        return *this;
    }
    operator QColor() const { return value(); }

    // query
    QColor value() const;

public Q_SLOTS:
    // modifier
    void setValue(const QColor &value);

Q_SIGNALS:
    void changed(const QColor&);

private:
    QSettings &m_settings;
    QString m_key;
    QString m_defaultColorName;
};

#endif // UTILITY_COLOR_SETTING_HPP
