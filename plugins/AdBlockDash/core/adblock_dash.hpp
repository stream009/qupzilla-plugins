#ifndef ADBLOCK_DASH_HPP
#define ADBLOCK_DASH_HPP

#include "common/path.hpp"
#include "downloader.hpp"
#include "updater.hpp"

#include <memory>
#include <utility>

#include <boost/container/flat_map.hpp>

#include <QtCore/QObject>

#include <adblock/api.h>

class QNetworkAccessManager;
class QNetworkRequest;
class QString;
class QUrl;
class QWebElement;
class QWebFrame;
class QWebPage;

class WebPage;

namespace adblock_dash {

class Settings;
class Subscription;
class PageAdaptor;

class AdBlockDash : public QObject
{
    Q_OBJECT
    using PageAdaptorMap =
        boost::container::flat_map<
            QWebPage*, std::unique_ptr<PageAdaptor>>;
public:
    // constructor
    AdBlockDash(Settings&, QNetworkAccessManager&);
    ~AdBlockDash();

    // query
    bool shouldBlock(const QString &url, const QWebElement&) const;
    bool shouldBlock(const QNetworkRequest&) const;
    bool shouldBlockWebSocket(const QString& url, const QWebFrame&) const;

    QString elementHideCss(const QUrl&) const;

    QMenu& pageMenu(QWebPage&) const;

    bool enabled() const { return m_enabled; }

    // modifiers
    void subscribe(const QString &title, const QUrl &location);

    void addFilterSet(const Path&);
    void removeFilterSet(const Path&);

    void setEnabled(bool);

public Q_SLOTS:
    void reload();
    void onWebPageCreated(WebPage*);
    void onWebPageDeleted(WebPage*);

Q_SIGNALS:
    void subscribed(const QString &title, const QUrl&);
    void subscribeError(const QString &title, const QUrl&,
                        const QString &message);
    void statusChanged(bool const enabled);

private:
    bool shouldBlock(const QString &url, const QWebFrame&,
                     const enum::content_type) const;
    void addFilterSets();

    Q_SLOT void onDownloadCompleted(const QString &title, const QUrl&,
                             const QString &path);
    Q_SLOT void onDownloadError(const QString &title, const QUrl&,
                         const QString &message);
    Q_SLOT void onSubscriptionAppended(const Subscription&);
    Q_SLOT void onDisableTriggered(bool const);

    void logBlockedRequest(
            const QWebFrame& frame,
            QUrl const& url,
            QString const& reason,
            QString const& subscription,
            ::adblock_context const& context) const;
private:
    adblock_t m_adBlock = nullptr;
    Settings &m_settings;
    QNetworkAccessManager &m_networkManager;
    Downloader m_downloader;
    Updater m_updater;
    PageAdaptorMap m_pageAdaptors;
    bool m_enabled = true;
};

} // namespace adblock_dash

#endif // ADBLOCK_DASH_HPP
