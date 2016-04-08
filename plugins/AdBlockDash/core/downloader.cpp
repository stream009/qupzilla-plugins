#include "downloader.hpp"

#include "settings.hpp"

#include <QtCore/QFile>

#include <followredirectreply.h>

namespace adblock_dash {

Downloader::
Downloader(const Settings &settings, QNetworkAccessManager &manager)
    : m_settings { settings },
      m_networkManager { manager }
{}

void Downloader::
operator()(const QString &title, const QUrl &location)
{
    if (m_downloader) {
        throw "downloading other subscription"; //TODO
    }

    m_title = title;
    m_downloader = new FollowRedirectReply(location, &m_networkManager);
    this->connect(m_downloader, SIGNAL(finished()),
                  this,           SLOT(onDownloadFinished()));
}

void Downloader::
onDownloadFinished()
{
    assert(this->sender() == m_downloader);

    struct Cleaner {
        Cleaner(Downloader &me) : self { me } {}
        ~Cleaner() {
            self.m_downloader->deleteLater();
            self.m_downloader = nullptr;
        }
        Downloader &self;
    } autoCleaner { *this };

    const auto &url = m_downloader->url();
    assert(url.isValid());

    if (m_downloader->error() != QNetworkReply::NoError) {
        Q_EMIT error(m_title, url, m_downloader->errorString());
        return;
    }

    const auto &filename = extractFilename(url);
    try {
        const auto &path = saveToFile(m_downloader->readAll(), filename);
        Q_EMIT finished(m_title, url, path);
    }
    catch (...) {
        Q_EMIT error(m_title, url, "save error"); //TODO
    }
}

static void
adjustExtension(QString &filename)
{
    if (!filename.endsWith(".txt")) {
        const auto pos = filename.lastIndexOf(".");
        filename =
            (pos != -1 ? filename.left(pos) : filename) + ".txt";
    }
}

QString Downloader::
extractFilename(const QUrl &url) const
{
    auto &&path = url.path();
    if (path.isEmpty() || path.endsWith("/")) {
        path = m_title + ".txt";
    }
    const auto pos = path.lastIndexOf("/");
    auto &&filename = pos != -1 ? path.mid(pos + 1) : path;

    adjustExtension(filename);

    return filename;
}

QString Downloader::
saveToFile(const QByteArray &content, const QString &filename) const
{
    const auto &path = m_settings.directory() + "/" + filename;

    QFile file { path };
    if (!file.open(QFile::ReadWrite | QFile::Truncate)) {
        throw "Unable to open file."; //TODO
    }

    file.write(content);
    file.close();

    return path;
}

} // namespace adblock_dash
