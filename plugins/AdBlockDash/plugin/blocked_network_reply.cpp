#include "blocked_network_reply.hpp"

#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QtNetwork/QNetworkRequest>

namespace adblock_dash {

BlockedNetworkReply::
BlockedNetworkReply(const QNetworkRequest &request, const QString &reason)
{
    Base::setRequest(request);
    Base::setUrl(request.url());
    Base::setError(QNetworkReply::ContentAccessDenied, reason);

    Base::open(QIODevice::ReadOnly);

    QTimer::singleShot(0, this, SLOT(delayedFinished()));
}

qint64 BlockedNetworkReply::
readData(char*, qint64)
{
    return -1;
}

void BlockedNetworkReply::
delayedFinished()
{
    Q_EMIT error(QNetworkReply::ContentAccessDenied);
    Q_EMIT finished();
}

} // namespace adblock_dash
