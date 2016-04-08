#ifndef ADBLOCK_DASH_HPP
#define ADBLOCK_DASH_HPP

#include "common/path.hpp"
#include "downloader.hpp"
#include "updater.hpp"

#include <utility>

#include <QtCore/QObject>

#include <adblock/api.h>

class QNetworkAccessManager;
class QNetworkRequest;
class QString;
class QUrl;
class QWebElement;
class QWebFrame;

class WebPage;

namespace adblock_dash {

class Settings;
class Subscription;

class AdBlockDash : public QObject
{
    Q_OBJECT
public:
    // constructor
    AdBlockDash(Settings&, QNetworkAccessManager&);
    ~AdBlockDash();

    // query
    bool shouldBlock(const QString &url, const QWebElement&) const;
    bool shouldBlock(const QNetworkRequest&) const;
    QString elementHideCss(const QUrl&) const;

    // modifiers
    void subscribe(const QString &title, const QUrl &location);

    void addFilterSet(const Path&);
    void removeFilterSet(const Path&);

public Q_SLOTS:
    void reload();
    void onWebPageCreated(WebPage*);

Q_SIGNALS:
    void subscribed(const QString &title, const QUrl&);
    void subscribeError(const QString &title, const QUrl&,
                        const QString &message);
private:
    void addFilterSets();

private Q_SLOTS:
    void onDownloadCompleted(const QString &title, const QUrl&,
                             const QString &path);
    void onDownloadError(const QString &title, const QUrl&,
                         const QString &message);
    void onFrameCreated(QWebFrame*);
    void onSubscriptionAppended(const Subscription&);

    void onUrlChanged(const QUrl&);

private:
    adblock_t m_adBlock = nullptr;
    Settings &m_settings;
    QNetworkAccessManager &m_networkManager;
    Downloader m_downloader;
    Updater m_updater;
};

} // namespace adblock_dash

#endif // ADBLOCK_DASH_HPP
