#ifndef ADBLOCK_DASH_UPDATER_HPP
#define ADBLOCK_DASH_UPDATER_HPP

#include <memory>
#include <unordered_map>

#include <QtCore/QObject>
#include <QtCore/QTimer>
#include <QtNetwork/QNetworkReply>

#include "settings.hpp"
#include "common/path.hpp"

class QNetworkAccessManager;

namespace adblock_dash {

class Updater : public QObject
{
    Q_OBJECT
public:
    Updater(Settings&, QNetworkAccessManager&);

    bool operator()() { return run(); }

Q_SIGNALS:
    void finished();
    void error(const Subscription&,
               const QNetworkReply::NetworkError);
private:
Q_SLOT bool run();
       bool update(Subscription&);
       void download(const QUrl&, Subscription&);
Q_SLOT void onDownloadFinished();
       void saveToFile(const QByteArray &content, const Path &path);

private:
    Settings &m_settings;
    QTimer m_timer;
    QNetworkAccessManager &m_networkManager;
    std::unordered_map<QNetworkReply*, Subscription*> m_ongoing;
};

} // namespace adblock_dash

#endif // ADBLOCK_DASH_UPDATER_HPP
