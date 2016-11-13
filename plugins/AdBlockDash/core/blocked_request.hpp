#ifndef ADBLOCK_DASH_BLOCKED_REQUEST_HPP
#define ADBLOCK_DASH_BLOCKED_REQUEST_HPP

#include "request_context.hpp"

#include <QtCore/QDateTime>
#include <QtCore/QUrl>
#include <QtCore/QString>
#include <QtCore/QDebug>

namespace adblock_dash {

struct BlockedRequest
{
    QDateTime time;
    QUrl url;
    QString rule;
    QString subscription;
    RequestContext context;
};

QDebug operator<<(QDebug debug, BlockedRequest const&);

} // namespace adblock_dash

#endif // ADBLOCK_DASH_BLOCKED_REQUEST_HPP
