#ifndef ADBLOCK_DASH_SUBSCRIPTION_HPP
#define ADBLOCK_DASH_SUBSCRIPTION_HPP

#include "common/path.hpp"
#include "filter_set.hpp"

#include <boost/optional.hpp>

#include <QtCore/QDateTime>
#include <QtCore/QFileSystemWatcher>
#include <QtCore/QString>
#include <QtCore/QUrl>

namespace adblock_dash {

class Subscription
{
public:
    Subscription(const QString &name, const Path &path,
                 const QUrl &location,
                 const bool enabled = true,
                 const bool updateStatus = true);
    Subscription() = default;

    // accessor
    const QString &name() const { return m_name; }
    const Path &path() const { return m_path; }
    bool enabled() const { return m_enabled; }
    const QUrl &location() const { return m_location; }
    const QDateTime &updated() const { return m_updated; }
    bool updateStatus() const { return m_updateStatus; }
    QUrl homePage() const;

    // query
    bool isExpired() const;
    bool hasIntegrity() const { return m_integrityChecked; }

    // modifier
    void setName(const QString &name) { m_name = name; }
    void setPath(const Path &path) { m_path = path; }
    void setEnabled(const bool enabled) { m_enabled = enabled; }
    void setUpdateStatus(const bool);

    // command
    void reload();

private:
    template<typename Str>
        void extractHeaderFields(Str&);

private:
    QString m_name;
    Path m_path;
    QUrl m_location;
    QDateTime m_updated;
    bool m_enabled;
    bool m_updateStatus;
    bool m_integrityChecked;

    boost::optional<QUrl> m_homePage;
    boost::optional<size_t> m_expires;
    boost::optional<QString> m_version;
};

} // namespace adblock_dash

#include "common/vector_setting.hpp"

#include <QtCore/QObject>

namespace adblock_dash {

class SubscriptionNotifier : public QObject
{
    Q_OBJECT
public:
    void emitAppended(const Subscription&);
    void emitRemoved(const Subscription&, const size_t index);
    void emitMoved(const Subscription&, const size_t from, const size_t to);

Q_SIGNALS:
    void appended(const Subscription&);
    void removed(const Subscription&, const size_t index);
    void moved(const Subscription&, const size_t from, const size_t to);
};

} // namespace adblock_dash

namespace vector_setting {

template<>
adblock_dash::Subscription load(QSettings &settings);

template<>
void save(QSettings&, const adblock_dash::Subscription&);

} // namespace vector_setting

#endif // ADBLOCK_DASH_SUBSCRIPTION_HPP
