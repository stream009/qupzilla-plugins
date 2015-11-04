#ifndef BOOKMARK_DASH_VIEW_P_H
#define BOOKMARK_DASH_VIEW_P_H

namespace bookmark_dash { namespace view {

class Slots
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

class QAction;
class QImage;
class QModelIndex;

namespace bookmark_dash { namespace view {

class SlotsDelegate : public QObject
{
    Q_OBJECT
public:
    SlotsDelegate(Slots&);

    void drop(const QString &title, const QUrl&, const QModelIndex &before);

    Q_SLOT void onActionImageReady(QAction*, const QImage&);

private Q_SLOTS:
    void onTimeout();

private:
    Slots &m_host;
    QString m_title;
    QUrl m_url;
    QModelIndex m_before;
};

}} // namespace bookmark_dash::view

#include <QtCore/QObject>
#include <QtCore/QRunnable>

class BookmarkItem;
class QAction;
class QImage;

namespace bookmark_dash { namespace view {

class IconImageLoader : public QObject, public QRunnable
{
    Q_OBJECT

public:
    IconImageLoader(BookmarkItem&, QAction&);

    // @override QRunnable
    void run();

Q_SIGNALS:
    void imageReady(QAction*, const QImage&);

private:
    BookmarkItem &m_item;
    QAction &m_action;
};

}} // namespace bookmark_dash::view

#endif // BOOKMARK_DASH_VIEW_P_H
