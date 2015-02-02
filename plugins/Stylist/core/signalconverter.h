#ifndef STYLIST_SIGNAL_CONVERTER_H
#define STYLIST_SIGNAL_CONVERTER_H

#include <boost/filesystem.hpp>

#include <QtCore/QObject>

namespace stylist {

class SignalConverter : public QObject
{ Q_OBJECT
    using Path = boost::filesystem::path;

Q_SIGNALS:
    void into(const Path &) const;
    void into(const QVariant &) const;

public Q_SLOTS:
    void from(const Path &) const;
    void from(const QVariant &) const;
};

} // namespace stylist

#endif // STYLIST_SIGNAL_CONVERTER_H
