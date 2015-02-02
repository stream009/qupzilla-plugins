#include "signalbuffer.h"

#include <QtCore/QDebug>

namespace stylist {

SignalBuffer::
SignalBuffer(const size_t interval /*= 300*/)
    : m_interval { interval }
{}

QTimer &SignalBuffer::
makeTimer()
{
    // We can't use smart pointer to manage lifetime of the
    // timer here. Because at the time of the receivng signal,
    // we can only retrieve signal sender as a raw pointer to the timer.
    // So, instead we use QObject (this) to manage lifetime of the timer.
    auto* const timer = new QTimer { this };
    assert(timer);
    timer->setSingleShot(true);
    timer->setInterval(m_interval);

    this->connect(timer, SIGNAL(timeout()),
                  this,  SLOT(slotTimerExpired()));
    return *timer;
}

void SignalBuffer::
slotTriggered(const QVariant &arg)
{
    //qDebug() << __func__ << arg;
    const auto it = m_timers.left.find(arg);
    if (it == m_timers.left.end()) {
        auto &timer = makeTimer();
        const auto &inserted =
            m_timers.left.insert(std::make_pair(arg, &timer));
        assert(inserted.second); //TODO better
        timer.start();
    }
    else {
        auto *timer = it->second;
        assert(timer);
        timer->start();
    }
}

void SignalBuffer::
slotTimerExpired()
{
    //qDebug() << __func__;
    auto* const timer = dynamic_cast<QTimer*>(this->sender());
    assert(timer);
    const auto it = m_timers.right.find(timer);
    assert(it != m_timers.right.end()); //TODO better
    const auto &arg = it->second;

    Q_EMIT triggered(arg);

    m_timers.right.erase(timer);
    timer->deleteLater();
}

} // namespace stylist
