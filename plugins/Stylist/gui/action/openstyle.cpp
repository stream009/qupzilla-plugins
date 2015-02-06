#include "openstyle.h"

#include <QtCore/QDebug>
#include <QtCore/QUrl>
#include <QtGui/QDesktopServices>

namespace stylist { namespace action {

OpenStyle::
OpenStyle(QObject* const parent/* = nullptr*/)
    : QAction { parent }
{}

void OpenStyle::
run(const Path &path)
{
    const QUrl url { path.c_str() };
    qDebug() << "Opening" << url;
    QDesktopServices::openUrl(url);
}

}} // namespace stylist::action
