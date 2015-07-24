#include "view_p.h"

#include <QtCore/QTimer>

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

}} // namespace bookmark_dash::view

