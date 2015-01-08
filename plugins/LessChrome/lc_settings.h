#ifndef LC_SETTINGS_H
#define LC_SETTINGS_H

#include <boost/type_traits.hpp>

#include <QSettings>

namespace lesschrome {

class Settings : public QObject
{
    Q_OBJECT
public:
    explicit Settings(const QString &filename);

signals:
    void change(QString);

private:
    template<typename T, const QString& Key, T Default>
    class Property {
    public:
        typedef typename boost::conditional<
                         boost::is_pod<T>::value, T, T&>::type TRef;

        Property(Settings* const parent) : m_p(parent) {}

        operator T() const {
            return m_p->m_settings.value(Key, Default).template value<T>();
        }

        void operator=(const TRef newValue) {
            if (operator T() != newValue) {
                m_p->m_settings.setValue(Key, newValue);
                emit m_p->change(Key);
            }
        }

    private:
        Settings *m_p;
    };

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

} // namespace lesschrome

#endif // LC_SETTINGS_H
