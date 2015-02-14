#include "signalconverter.h"

#include "common/utility.h"

namespace stylist {

void SignalConverter::
from(const Path &path) const
{
    //qDebug() << __func__ << path;
    Q_EMIT into(QVariant::fromValue(path));
}

void SignalConverter::
from(const QVariant &variant) const
{
    //qDebug() << __func__ << variant;
    assert(variant.canConvert<Path>());
    Q_EMIT into(variant.value<Path>());
}

} // namespace stylist
