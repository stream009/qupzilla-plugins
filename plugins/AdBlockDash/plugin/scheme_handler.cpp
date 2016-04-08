#include "scheme_handler.hpp"

#include "core/adblock_dash.hpp"

#include <boost/container/flat_map.hpp>
#include <boost/range/algorithm.hpp>

#include <QtCore/QUrlQuery>
#include <QtWidgets/QMessageBox>

#include <emptynetworkreply.h>

namespace adblock_dash {

using Args = boost::container::flat_map<QString, QString>;

Args
extractArguments(const QUrl &url)
{
    const auto &list =
#if QT_VERSION >= 0x050000
        QUrlQuery(url).queryItems();
#else
        url.queryItems();
#endif
    static const QString allowedKeys[] = {
        "title", "location",
        "requiresTitle", "requiresLocation",
    };

    Args result;
    for (const auto &pair: list) {
        const auto &key = pair.first;
        const auto &value  = pair.second;

        if (key == "location" || key == "requiresLocation") {
            const QUrl url { value };
            if (!url.isValid()) {
                throw "invalid URL"; //TODO
            }
        }

        if (boost::find(allowedKeys, key) == boost::end(allowedKeys)) {
            throw "unknown key"; //TODO
        }

        const auto rv = result.insert(std::make_pair(key, value));
        if (!rv.second) {
            throw "duplicate key"; //TODO
        }
    }

    const auto &contains = [] (const Args &args, const char key[]) {
        return args.find(key) != args.end();
    };

    if (!contains(result, "title") || !contains(result, "location")) {
        throw "required argument is missing"; //TODO
    }

    if (contains(result, "requiresTitle") &&
            !contains(result, "requiresLocation"))
    {
        throw "required argument is missing"; //TODO
    }

    return result;
}

static QWidget *
parentWidget(const QNetworkRequest &request)
{
    auto* const frame = dynamic_cast<QWebFrame*>(request.originatingObject());
    if (!frame) return nullptr;

    auto* const page = frame->page();
    assert(page);

    return page->view(); // still chould be nullptr
}

SchemeHandler::
SchemeHandler(NetworkManager &manager, AdBlockDash &adblockDash)
    : m_networkManager { manager },
      m_adBlockDash { adblockDash }
{
    m_networkManager.registerSchemeHandler("abp", this);
}

SchemeHandler::
~SchemeHandler()
{
    m_networkManager.unregisterSchemeHandler("abp", this);
}

QNetworkReply *SchemeHandler::
createRequest(QNetworkAccessManager::Operation op,
              const QNetworkRequest &request, QIODevice*)
{
    if (op != QNetworkAccessManager::GetOperation) {
        return nullptr;
    }

    const auto &url = request.url();
    assert(url.scheme() == "abp");

    const auto &args = extractArguments(url); //TODO catch

    const auto &title = args.at("title");
    assert(!title.isNull());
    const QUrl location { args.at("location") };
    assert(location.isValid());

    const auto &rv = QMessageBox::question(
        parentWidget(request),
        tr("AdBlock Subscription"),
        tr("Do you want to add <b>%1</b> subscription?").arg(title),
        QMessageBox::Yes | QMessageBox::No
    );
    if (rv == QMessageBox::Yes) {
        qDebug() << title << location;
        m_adBlockDash.subscribe(title, location);
    }

    return new EmptyNetworkReply;
}

} // namespace adblock_dash
