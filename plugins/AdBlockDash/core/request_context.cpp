#include "request_context.hpp"

namespace adblock_dash {

QString
toString(RequestContext::ContentType const contentType)
{
    switch (contentType) {
    case RequestContext::ContentType::TYPE_OTHER:
        return "other";
    case RequestContext::ContentType::TYPE_SCRIPT:
        return "script";
    case RequestContext::ContentType::TYPE_IMAGE:
        return "image";
    case RequestContext::ContentType::TYPE_STYLESHEET:
        return "style sheet";
    case RequestContext::ContentType::TYPE_OBJECT:
        return "object";
    case RequestContext::ContentType::TYPE_DOCUMENT:
        return "document";
    case RequestContext::ContentType::TYPE_SUBDOCUMENT:
        return "subdocument";
    case RequestContext::ContentType::TYPE_XMLHTTPREQUEST:
        return "XMLHTTPRequest";
    case RequestContext::ContentType::TYPE_OBJECT_SUBREQUEST:
        return "object subrequest";
    case RequestContext::ContentType::TYPE_MEDIA:
        return "font";
    case RequestContext::ContentType::TYPE_FONT:
        return "media";
    case RequestContext::ContentType::TYPE_WEBSOCKET:
        return "websocket";
    default:
        assert(false && "unknown content type");
    }
}

} // namespace adblock_dash
