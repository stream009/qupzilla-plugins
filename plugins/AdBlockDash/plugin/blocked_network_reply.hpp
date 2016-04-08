#ifndef ADBLOCK_DASH_BLOCKED_NETWORK_REPLY_HPP
#define ADBLOCK_DASH_BLOCKED_NETWORK_REPLY_HPP

#include <QtNetwork/QNetworkReply>

class QNetworkRequest;
class QString;

namespace adblock_dash {

class BlockedNetworkReply : public QNetworkReply
{
    Q_OBJECT
    using Base = QNetworkReply;
public:
    BlockedNetworkReply(const QNetworkRequest&, const QString &reason);

    //@overide QNetworkReply
    void abort() override {}

protected:
    //@overide QIODevice
    qint64 readData(char *data, qint64 maxSize) override;

private Q_SLOTS:
    void delayedFinished();
};

} // namespace adblock_dash

#endif // ADBLOCK_DASH_BLOCKED_NETWORK_REPLY_HPP
