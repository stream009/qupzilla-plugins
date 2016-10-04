#include "subscription.hpp"

#include "common/utility.hpp"

#include <fstream>
#include <iterator>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/range/iterator_range.hpp>
#include <boost/regex.hpp>

#include <QtCore/QCryptographicHash>

namespace adblock_dash {

template<typename Str>
static Str loadFile(const Path &path);

template<typename Str>
static bool checkIntegrity(const Path &, Str &buf);

template<typename Str>
static QByteArray extractChecksum(Str &buf);

template<typename Str>
static void normalize(Str &buf);

template<typename Str>
static QByteArray md5Hash(Str &buf);

static MetaTypeRegistration<Subscription> registration { "Subscription" };


Subscription::
Subscription(const QString &name, const Path &path,
             const QUrl &location,
             const bool enabled/*= true*/,
             const bool updateStatus/*= true*/)
    : m_name { name }
    , m_path { path }
    , m_location { location }
    , m_enabled { enabled }
    , m_updateStatus { updateStatus }
{
    assert(!m_name.isNull());
    assert(m_location.isValid());

    reload();
}

QUrl Subscription::
homePage() const
{
    return m_homePage ? m_homePage.get() : QUrl {};
}

bool Subscription::
isExpired() const
{
    if (!boost::filesystem::exists(m_path)) return true;

    if (!m_expires) return false;

    const auto &expiration =
            m_updated.addSecs(::toSignedInt(m_expires.get()) * 60 * 60);

    return QDateTime::currentDateTime() >= expiration;
}

void Subscription::
setUpdateStatus(const bool status)
{
    m_updateStatus = status;
}

void Subscription::
reload()
{
    namespace bfs = boost::filesystem;

    const auto &path = this->path();
    if (!bfs::exists(path)) return; // It is regarded as expired, then will be downloaded.

    m_updated = QDateTime::fromTime_t(
        static_cast<uint>(bfs::last_write_time(path)));

    auto &&buf = loadFile<std::string>(path);

    m_integrityChecked = checkIntegrity(path, buf);

    extractHeaderFields(buf);
}

template<typename Str>
void Subscription::
extractHeaderFields(Str &buf)
{
    boost::match_results<typename Str::iterator> match;
    bool rc = false;

    const auto flag = boost::match_not_dot_newline;

    static boost::regex homePageRe {
        R"(\s*!\s*Homepage:\s*(.+))"
    };
    rc = boost::regex_search(buf.begin(), buf.end(), match, homePageRe, flag);
    if (rc) {
        assert(match.size() == 2);

        auto* const c_str =
                buf.data() + std::distance(buf.begin(), match[1].first);
        const auto size = std::distance(match[1].first, match[1].second);

        m_homePage = QString::fromUtf8(c_str, size);
        assert(m_homePage->isValid());
    }

    static boost::regex expiresRe {
        R"(\s*!\s*Expires:\s*(\d)+\s*(days|hours))"
    };
    rc = boost::regex_search(buf.begin(), buf.end(), match, expiresRe, flag);
    if (rc) {
        assert(match.size() == 3);

        char *end = nullptr;
        auto* const c_str =
                buf.data() + std::distance(buf.begin(), match[1].first);
        m_expires = std::strtoul(c_str, &end, 10);
        if (*match[2].first == 'd') { // days
            *m_expires *= 24;
        }
    }

    static boost::regex versionRe {
        R"(\s*!\s*Version:\s*([[:digit:]]+))"
    };
    rc = boost::regex_search(buf.begin(), buf.end(), match, versionRe, flag);
    if (rc) {
        assert(match.size() == 2);

        auto* const c_str =
                buf.data() + std::distance(buf.begin(), match[1].first);
        const auto size = std::distance(match[1].first, match[1].second);

        m_version = QString::fromUtf8(c_str, size);
    }
}


template<typename Str>
static Str
loadFile(const Path &path)
{
    namespace bfs = boost::filesystem;

    bfs::ifstream ifs { path };
    assert(ifs.is_open());
    const auto size = bfs::file_size(path);

    Str result;
    result.reserve(static_cast<size_t>(size));
    result.assign(std::istreambuf_iterator<char>(ifs), {});

    return result;
}

template<typename Str>
static bool
checkIntegrity(const Path &path, Str &buf)
{
    const auto &checksum = extractChecksum(buf);
    if (checksum.isNull()) return false;

    normalize(buf);

    const auto &md5Digest = md5Hash(buf);

    if (checksum != md5Digest) {
        qDebug() << path.c_str() << ":"
                 << checksum.toBase64() << "vs" << md5Digest.toBase64();
        return false;
    }

    return true;
}

template<typename Str>
static QByteArray
extractChecksum(Str &buf)
{
    boost::match_results<typename Str::iterator> match;

    static boost::regex re {
        R"(^\s*!\s*Checksum:\s*([0-9a-zA-Z+/]+).*?$\n)"
    };
    const auto rc =
            boost::regex_search(buf.begin(), buf.end(), match, re);
    if (!rc) {
        return {};
    }

    assert(match.size() == 2);

    Str checksum { match[1].first, match[1].second };

    buf.erase(match[0].first, match[0].second);

    return QByteArray::fromBase64(
                { checksum.data(), ::toSignedInt(checksum.size()) });
}

template<typename Str>
static void
normalize(Str &buf)
{
    Str tmp;
    tmp.reserve(buf.size());

    assert(!buf.empty());
    auto const last = buf.size()-1;
    for (auto i = 0u; i <= last; ++i) {
        if (buf[i] == '\r') continue;
        if (buf[i] == '\n') {
            if (i != last && buf[i+1] == '\n') continue;
        }

        tmp.push_back(buf[i]);
    }

    buf.swap(tmp);
}


template<typename Str>
static QByteArray
md5Hash(Str &buf)
{
    QCryptographicHash hash { QCryptographicHash::Md5 };
    hash.addData(buf.data(), ::toSignedInt(buf.size()));

    return hash.result();
}

void SubscriptionNotifier::
emitAppended(const Subscription &value)
{
    Q_EMIT appended(value);
}

void SubscriptionNotifier::
emitRemoved(const Subscription &value, const size_t index)
{
    Q_EMIT removed(value, index);
}

void SubscriptionNotifier::
emitMoved(const Subscription &value, const size_t from, const size_t to)
{
    Q_EMIT moved(value, from, to);
}

} // namespace adblock_dash

namespace vector_setting {

template<>
adblock_dash::Subscription
load(QSettings &settings)
{
    namespace bfs = boost::filesystem;

    const auto &name = settings.value("name").toString();
    assert(!name.isEmpty());

    const auto &pathStr = settings.value("path").toString();
    const bfs::path &path { pathStr.toUtf8().constData() };
    if (!bfs::exists(path)) {
        assert(bfs::exists(path)); //TODO better
    }

    const auto enabled = settings.value("enabled", true).toBool();

    const QUrl location { settings.value("location").toString() };
    assert(location.isValid());

    const auto &updateStatus = settings.value("updateStatus").toBool();

    return { name, path, location, enabled, updateStatus };
}

template<>
void
save(QSettings &settings, const adblock_dash::Subscription &value)
{
    settings.setValue("name", value.name());
    settings.setValue("path", QString { value.path().c_str() });
    settings.setValue("enabled", value.enabled());
    settings.setValue("location", value.location().toString());
    settings.setValue("updateStatus", value.updateStatus());
}

} // namespace vector_setting

