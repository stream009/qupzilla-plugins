#ifndef STYLIST_SIGNALBUFFER_H
#define STYLIST_SIGNALBUFFER_H

#include "common/utility.h"

#include <memory>

#include <boost/bimap.hpp>
#include <boost/bimap/unordered_set_of.hpp>

#include <QtCore/QTimer>
#include <QtCore/QVariant>

namespace stylist {

class SignalBuffer : public QObject
{
    Q_OBJECT
    using TimerMap = boost::bimap<boost::bimaps::unordered_set_of<QVariant>,
                                  boost::bimaps::unordered_set_of<QTimer*>>;
public:
    SignalBuffer(const size_t interval = 300);

private:
    QTimer &makeTimer();

Q_SIGNALS:
    void triggered(const QVariant&);

public Q_SLOTS:
    void slotTriggered(const QVariant&);

private Q_SLOTS:
    void slotTimerExpired();

private:
    const size_t m_interval;
    TimerMap m_timers;
};

} // namespace stylist

#endif // STYLIST_SIGNALBUFFER_H
