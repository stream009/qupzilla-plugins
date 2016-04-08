#include "updater.hpp"

#include "common/utility.hpp"

#include <cassert>
#include <iterator>

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/regex.hpp>

#include <QtCore/QByteArray>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>

static QUrl
redirectUrl(const char* const buf, const size_t len)
{
    static boost::regex re {
        R"(\s*!\s*Redirect:\s*([^\s\n]*)$)"
    };

    boost::cmatch match;
    const auto rc = boost::regex_search(
        buf, buf + len,
        match, re
    );
    if (!rc) return {};

    assert(match.size() == 2);
    auto* const cUrl = match[1].first;
    const auto cUrlLen = std::distance(match[1].first, match[1].second);

    return QString::fromUtf8(cUrl, cUrlLen);
}


namespace adblock_dash {

Updater::
Updater(Settings &settings, QNetworkAccessManager &networkManager)
    : m_settings { settings }
    , m_networkManager { networkManager }
{
    m_timer.setInterval(1 * 60 * 60 * 1000); // a hour
    m_timer.setSingleShot(false);
    this->connect(&m_timer, SIGNAL(timeout()),
                  this,       SLOT(run()));
}

bool Updater::
run()
{
    bool updateAvailable = false;

    for (auto &subscription: m_settings.subscriptions()) {
        if (update(subscription)) {
            updateAvailable = true;
        }
    }

    auto &exceptionRules = m_settings.exceptionRules();
    if (exceptionRules.enabled()) {
        if (update(exceptionRules)) {
            updateAvailable = true;
        }
    }

    return updateAvailable;
}

bool Updater::
update(Subscription &subscription)
{
    if (!subscription.isExpired()) return false;

    qDebug() << QString {
        "Subscription (%1) is expired. Start downloading"
    }.arg(subscription.name());

    const QUrl url { subscription.location() };
    assert(url.isValid());

    download(url, subscription);

    return true;
}

void Updater::
download(const QUrl &url, Subscription &subscription)
{
    QNetworkRequest req { url };

    auto* const reply = m_networkManager.get(req);
    assert(reply);
    this->connect(reply, SIGNAL(finished()),
                  this,    SLOT(onDownloadFinished()));

    assert(m_ongoing.count(reply) == 0);
    m_ongoing[reply] = &subscription;
}

void Updater::
onDownloadFinished()
{
    auto* const reply = dynamic_cast<QNetworkReply*>(this->sender());
    assert(reply);

    auto* const subscription = m_ongoing.at(reply);
    assert(subscription);

    qDebug() << QString {
        "Download of \"%1\" has finished. Error code: %2"
    }.arg(subscription->name()).arg(reply->error());

    if (reply->error() == QNetworkReply::NoError) {
        const auto &content = reply->readAll();
        const auto &url =
            redirectUrl(content.constData(), ::toSizeT(content.size()));

        if (url.isValid()) {
            m_ongoing.erase(reply);
            reply->deleteLater();

            qDebug() << "Redirecting to " << url;
            download(url, *subscription);
            return;
        }
        else {
            saveToFile(content, subscription->path());
            subscription->setUpdateStatus(true);
            subscription->reload();
        }
    }
    else {
        subscription->setUpdateStatus(false);
        Q_EMIT error(*subscription, reply->error());
    }

    m_ongoing.erase(reply);
    reply->deleteLater();

    if (m_ongoing.empty()) {
        Q_EMIT finished();
    }
}

void Updater::
saveToFile(const QByteArray &content, const Path &path)
{
    namespace bfs = boost::filesystem;

    bfs::ofstream ofs { path, std::ios_base::out | std::ios_base::trunc };
    assert(ofs.is_open());

    ofs.write(content.constData(), content.size());

    ofs.close();

    assert(bfs::file_size(path) == ::toSizeT(content.size()));
}

} // namespace adblock_dash
