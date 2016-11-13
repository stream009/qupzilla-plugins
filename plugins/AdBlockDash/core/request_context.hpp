#ifndef ADBLOCK_DASH_REQUEST_CONTEXT_HPP
#define ADBLOCK_DASH_REQUEST_CONTEXT_HPP

#include <QtCore/QUrl>
#include <QtCore/QString>

#include <adblock/api.h>

struct adblock_context;

namespace adblock_dash {

struct RequestContext
{
    using ContentType = ::content_type;

    QUrl origin;
    QString siteKey;
    ContentType contentType;
    bool isPopup;

    RequestContext(::adblock_context const&);
};

QString toString(RequestContext::ContentType const);

} // namespace adblock_dash

#endif // ADBLOCK_DASH_REQUEST_CONTEXT_HPP
