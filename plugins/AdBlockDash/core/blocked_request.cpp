#include "blocked_request.hpp"

namespace adblock_dash {

QDebug operator<<(QDebug debug, BlockedRequest const &req)
{
    QDebugStateSaver saver { debug };

    debug << "Time:" << req.time.toString() << "\n"
          << "URL:" << req.url.toString() << "\n"
          << "Rule:" << req.rule << "\n"
          << "Subscription:" << req.subscription << "\n"
          << "Origin:" << req.context.origin.toString() << "\n"
          << "Site key:" << req.context.siteKey << "\n"
          << "Content Type:" << toString(req.context.contentType) << "\n"
          << "Popup:" << req.context.isPopup;

    return debug;
}

} // namespace adblock_dash
