#ifndef UTILITY_SETTING_HPP
#define UTILITY_SETTING_HPP

#include <QtCore/QObject>

class QSettings;

class BoolSetting : public QObject
{
    Q_OBJECT
public:
    // constructor
    BoolSetting(QSettings&, const char key[], const bool defaultValue);

    // operator
    BoolSetting &operator=(const bool value)
    {
        setValue(value);
        return *this;
    }
    operator bool() const { return value(); }

    // query
    bool value() const;

public Q_SLOTS:
    // modifier
    void setValue(const bool value);

Q_SIGNALS:
    void changed(bool);

private:
    QSettings &m_settings;
    const char* const m_key;
    bool m_default;
};


#include <QtCore/QObject>

class QSettings;
class QString;

class StringSetting : public QObject
{
    Q_OBJECT
public:
    // constructor
    StringSetting(QSettings&,
            const QString &key, const QString &defaultValue);

    // operator
    StringSetting &operator=(const QString &value)
    {
        setValue(value);
        return *this;
    }
    operator QString() const { return value(); }

    // query
    QString value() const;

public Q_SLOTS:
    // modifier
    void setValue(const QString &value);

Q_SIGNALS:
    void changed(const QString&);

private:
    QSettings &m_settings;
    QString m_key;
    QString m_defaultValue;
};

#endif // UTILITY_SETTING_HPP
