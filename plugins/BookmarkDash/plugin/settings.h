#ifndef BOOKMARK_DASH_SETTINGS_H
#define BOOKMARK_DASH_SETTINGS_H

#include <cassert>

#include <boost/type_traits.hpp>

#include <QtCore/QSettings>
#include <QtCore/QVariant>

namespace bookmark_dash {

class Settings : public QObject
{
    Q_OBJECT
private:
    template<typename T, const QString& Key, T Default>
    class Property {
    public:
        using TRef = typename boost::conditional<
                         boost::is_pod<T>::value, T, T&>::type;

        Property(Settings &parent) : m_p { parent } {}

        operator T() const {
            const auto &value = m_p.m_settings.value(Key, Default);
            assert(value.template canConvert<T>());
            return value.template value<T>();
        }

        void operator=(const TRef newValue) {
            if (operator T() != newValue) {
                m_p.m_settings.setValue(Key, newValue);
                Q_EMIT m_p.change(Key);
            }
        }

    private:
        Settings &m_p;
    };

public:
    explicit Settings(const QString &filename);

Q_SIGNALS:
    void change(QString);

public: // static
    static const QString keyNavigationBar;
    static const QString keyBookmarksBar;
    static const QString keyStatusBar;
    static const QString keyMenuBar;
    static const QString keyMenuBarKey;
    static const QString keyWaitTimer;

public: // Yes, public member variable. Sue me.
    Property<bool, keyNavigationBar, true> navigationBar;
    Property<bool, keyBookmarksBar, true> bookmarksBar;
    Property<bool, keyStatusBar, true> statusBar;
    Property<bool, keyMenuBar, true> menuBar;
    Property<int, keyMenuBarKey, Qt::Key_Alt> menuBarKey;
    Property<size_t, keyWaitTimer, 1000> waitTimer;

private:
    QSettings m_settings;
};

} // namespace bookmark_dash

#endif // BOOKMARK_DASH_SETTINGS_H
