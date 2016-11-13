#ifndef ADBLOCK_DASH_SCHEME_HANDLER_HPP
#define ADBLOCK_DASH_SCHEME_HANDLER_HPP

#include <QtNetwork/QNetworkAccessManager>

#include <qzcommon.h>
#include <schemehandlers/schemehandler.h>

class QIODevice;
class QNetworkReply;
class QNetworkRequest;

class NetworkManager;

namespace adblock_dash {

class AdBlockDash;

class SchemeHandler
    : public QObject, public ::SchemeHandler
{
public:
    explicit SchemeHandler(NetworkManager&, AdBlockDash&);
    ~SchemeHandler() override;

    QNetworkReply *createRequest(
            QNetworkAccessManager::Operation,
            const QNetworkRequest&, QIODevice* outgoingData);

private:
    NetworkManager &m_networkManager;
    AdBlockDash &m_adBlockDash;
};

} // namespace adblock_dash

#endif // ADBLOCK_DASH_SCHEME_HANDLER_HPP
