#ifndef ADBLOCK_DASH_DOWNLOADER_HPP
#define ADBLOCK_DASH_DOWNLOADER_HPP

#include <QtCore/QObject>

class QNetworkAccessManager;
class QUrl;

class FollowRedirectReply;

namespace adblock_dash {

class Settings;

class Downloader : public QObject
{
    Q_OBJECT
public:
    // constructors
    Downloader(const Settings&, QNetworkAccessManager&);

    // operators
    void operator()(const QString &title, const QUrl&);

Q_SIGNALS:
    void finished(const QString &title, const QUrl&,
                  const QString &path);
    void error(const QString &title, const QUrl&, const QString &message);

private Q_SLOTS:
    void onDownloadFinished();

private:
    QString extractFilename(const QUrl&) const;
    QString saveToFile(const QByteArray &content,
                       const QString &filename) const;
private:
    const Settings &m_settings;
    QNetworkAccessManager &m_networkManager;

    FollowRedirectReply *m_downloader = nullptr;
    QString m_title;
};

} // namespace adblock_dash

#endif // ADBLOCK_DASH_DOWNLOADER_HPP
