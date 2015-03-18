#include "view_p.h"

#include <QtCore/QTimer>

namespace bookmark_dash { namespace view {

UrlDropHandler::
UrlDropHandler(UrlDrop &host)
    : m_host { host }
{}

void UrlDropHandler::
drop(const QString &title, const QUrl &url, const QModelIndex &index)
{
    assert(url.isValid());
    assert(index.isValid());

    m_title = title;
    m_url = url;
    m_index = index;

    QTimer::singleShot(0, this, SLOT(onTimeout()));
}

void UrlDropHandler::
onTimeout()
{
    assert(m_url.isValid());
    assert(m_index.isValid());

    m_host.onUrlDropped(m_title, m_url, m_index);
}

}} // namespace bookmark_dash::view

