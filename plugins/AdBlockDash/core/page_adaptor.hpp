#ifndef ADBLOCK_DASH_PAGE_ADAPTOR_HPP
#define ADBLOCK_DASH_PAGE_ADAPTOR_HPP

#include <vector>

#include <QtCore/QObject>

class QDateTime;
class QString;
class QWebPage;
class QWebFrame;
class QUrl;

struct adblock_context;

namespace adblock_dash {

class Subscription;

struct RequestContext
{
    using ContentType = ::content_type;

    QUrl origin;
    QString siteKey;
    ContentType contentType;
    bool isPopup;

    RequestContext(::adblock_context const&);
};

struct BlockedRequest
{
    QDateTime time;
    QUrl url;
    QString rule;
    QString subscription;
    RequestContext context;
};

class AdBlockDash;

class PageAdaptor : public QObject
{
    Q_OBJECT
    using Base = QObject;
    using BlockedRequests = std::vector<BlockedRequest>;
public:
    PageAdaptor(QWebPage&, AdBlockDash&);

    BlockedRequests& blockedRequests() { return m_blockedRequests; }

    Q_SLOT void dump() const;

private:
    Q_SLOT void onFrameCreated(QWebFrame*);
    Q_SLOT void onUrlChanged(QUrl const&);

private:
    QWebPage &m_page;
    AdBlockDash &m_adBlockDash;
    BlockedRequests m_blockedRequests;
};

} // namespace adblock_dash

#endif // ADBLOCK_DASH_PAGE_ADAPTOR_HPP
