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
    class Item {
    public:
        typedef typename boost::conditional<
                         boost::is_pod<T>::value, T, T&>::type TRef;

        Item(Settings* const parent) : m_p(parent) {}

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
    static const QString keyWaitTimer;

public: // Yes, public member variable. Sue me.
    Item<bool, keyNavigationBar, true> navigationBar;
    Item<bool, keyBookmarksBar, true> bookmarksBar;
    Item<bool, keyStatusBar, true> statusBar;
    Item<size_t, keyWaitTimer, 1000> waitTimer;

private:
    QSettings m_settings;
};

} // namespace lesschrome

#endif // LC_SETTINGS_H
