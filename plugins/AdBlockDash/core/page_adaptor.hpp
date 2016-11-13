#ifndef ADBLOCK_DASH_PAGE_ADAPTOR_HPP
#define ADBLOCK_DASH_PAGE_ADAPTOR_HPP

#include "blocked_request.hpp"

#include <vector>
#include <memory>

#include <boost/container/flat_map.hpp>

#include <QtCore/QObject>

class QWebPage;
class QWebFrame;
class QUrl;

namespace adblock_dash {

class AdBlockDash;
class FrameAdaptor;

class PageAdaptor : public QObject
{
    Q_OBJECT
    using Base = QObject;
    using BlockedRequests = std::vector<BlockedRequest>;
    using FrameAdaptorMap =
        boost::container::flat_map<
            QWebFrame*, std::unique_ptr<FrameAdaptor>>;
public:
    PageAdaptor(QWebPage&, AdBlockDash&);
    ~PageAdaptor() override;

    BlockedRequests& blockedRequests() { return m_blockedRequests; }

    Q_SLOT void dump() const;

private:
    Q_SLOT void onFrameCreated(QWebFrame*);
    Q_SLOT void onFrameDestroyed();
    Q_SLOT void onUrlChanged(QUrl const&);

private:
    QWebPage &m_page;
    AdBlockDash &m_adBlockDash;
    BlockedRequests m_blockedRequests;
    FrameAdaptorMap m_frameAdaptors;
};

} // namespace adblock_dash

#endif // ADBLOCK_DASH_PAGE_ADAPTOR_HPP
