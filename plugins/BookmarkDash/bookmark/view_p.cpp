#include "view_p.h"

#include <QtCore/QTimer>
#include <QtGui/QImage>
#include <QtGui/QPixmap>
#include <QtWidgets/QAction>

namespace bookmark_dash { namespace view {

SlotsDelegate::
SlotsDelegate(Slots &host)
    : m_host { host }
{}

void SlotsDelegate::
drop(const QString &title, const QUrl &url, const QModelIndex &before)
{
    assert(url.isValid());
    // before can be invalid.

    m_title = title;
    m_url = url;
    m_before = before;

    QTimer::singleShot(0, this, SLOT(onTimeout()));
}

void SlotsDelegate::
onTimeout()
{
    assert(m_url.isValid());

    m_host.onUrlDropped(m_title, m_url, m_before);
}

void SlotsDelegate::
onActionImageReady(QAction* const action, const QImage &image)
{
    assert(action);
    action->setIcon(QPixmap::fromImage(image));
}

}} // namespace bookmark_dash::view


#include <QtWidgets/QAction>

#include <bookmarkitem.h>
#include <iconprovider.h>

namespace bookmark_dash { namespace view {

IconImageLoader::
IconImageLoader(BookmarkItem &item, QAction &action)
    : m_item { item }
    , m_action { action }
{}

void IconImageLoader::
run()
{
    const auto &image = IconProvider::imageForUrl(m_item.url());

    Q_EMIT imageReady(&m_action, image);
}

}} // namespace bookmark_dash::view
