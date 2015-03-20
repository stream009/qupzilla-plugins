#ifndef BOOKMARK_DASH_VIEW_P_H
#define BOOKMARK_DASH_VIEW_P_H

namespace bookmark_dash { namespace view {

class UrlDrop
{
public:
    virtual void onUrlDropped(
        const QString &title, const QUrl&, const QModelIndex &before) = 0;
};

}} // namespace bookmark_dash::view

#include <QtCore/QModelIndex>
#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QUrl>

class QModelIndex;

namespace bookmark_dash { namespace view {

class UrlDropHandler : public QObject
{
    Q_OBJECT
public:
    UrlDropHandler(UrlDrop&);

    void drop(const QString &title, const QUrl&, const QModelIndex &before);

private Q_SLOTS:
    void onTimeout();

private:
    UrlDrop &m_host;
    QString m_title;
    QUrl m_url;
    QModelIndex m_before;
};

}} // namespace bookmark_dash::view

#endif // BOOKMARK_DASH_VIEW_P_H
