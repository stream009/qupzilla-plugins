#include "openstyle.h"

#include <QtCore/QUrl>
#include <QtCore/QDebug>
#include <QtGui/QDesktopServices>

namespace stylist { namespace action {

OpenStyle::
OpenStyle(QObject* const parent/* = nullptr*/)
    : QAction { tr("&Open"), parent }
{}

void OpenStyle::
run(const Path &path) const
{
    const QUrl url { path.c_str() };
    qDebug() << "Opening" << url;
    QDesktopServices::openUrl(url);
}

}} // namespace stylist::action
