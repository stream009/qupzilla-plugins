#include "signalbuffertest.h"

#include "../signalbuffer.h"

#include <QtCore/QDebug>
#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>

namespace stylist {

void SignalBufferTest::
testConstructor()
{
    SignalBuffer buffer;
}

void SignalBufferTest::
testBuffering()
{
    SignalBuffer buffer;
    this->connect(this,    SIGNAL(fire(const QVariant&)),
                  &buffer, SLOT(slotTriggered(const QVariant&)));
    QSignalSpy spy { &buffer, SIGNAL(triggered(const QVariant&)) };

    Q_EMIT fire("first");

    QTest::qWait(100);
    QCOMPARE(spy.size(), 0);

    QTest::qWait(300);
    QCOMPARE(spy.size(), 1);
    const auto args = spy.takeFirst();
    QCOMPARE(args.size(), 1);
    QCOMPARE(static_cast<QMetaType::Type>(args[0].type()), QMetaType::QVariant);
    QCOMPARE(args[0].value<QVariant>(), QVariant { "first" });
}

void SignalBufferTest::
testBuffering2()
{
    SignalBuffer buffer;
    this->connect(this,    SIGNAL(fire(const QVariant&)),
                  &buffer, SLOT(slotTriggered(const QVariant&)));
    QSignalSpy spy { &buffer, SIGNAL(triggered(const QVariant&)) };

    Q_EMIT fire("first");
    Q_EMIT fire("first");
    Q_EMIT fire("first");
    Q_EMIT fire("first");

    QTest::qWait(100);
    QCOMPARE(spy.size(), 0);

    QTest::qWait(300);
    QCOMPARE(spy.size(), 1);
    const auto args = spy.takeFirst();
    QCOMPARE(args.size(), 1);
    QCOMPARE(static_cast<QMetaType::Type>(args[0].type()), QMetaType::QVariant);
    QCOMPARE(args[0].value<QVariant>(), QVariant { "first" });
}

void SignalBufferTest::
testBuffering3()
{
    SignalBuffer buffer;
    this->connect(this,    SIGNAL(fire(const QVariant&)),
                  &buffer, SLOT(slotTriggered(const QVariant&)));
    QSignalSpy spy { &buffer, SIGNAL(triggered(const QVariant&)) };

    Q_EMIT fire("first");
    Q_EMIT fire("first");
    Q_EMIT fire("second");
    Q_EMIT fire("third");

    QTest::qWait(100);
    QCOMPARE(spy.size(), 0);

    QTest::qWait(300);
    QCOMPARE(spy.size(), 3);

    auto args = spy.takeFirst();
    QCOMPARE(args.size(), 1);
    QCOMPARE(static_cast<QMetaType::Type>(args[0].type()), QMetaType::QVariant);
    QCOMPARE(args[0].value<QVariant>(), QVariant { "first" });

    args = spy.takeFirst();
    QCOMPARE(args.size(), 1);
    QCOMPARE(static_cast<QMetaType::Type>(args[0].type()), QMetaType::QVariant);
    QCOMPARE(args[0].value<QVariant>(), QVariant { "second" });

    args = spy.takeFirst();
    QCOMPARE(args.size(), 1);
    QCOMPARE(static_cast<QMetaType::Type>(args[0].type()), QMetaType::QVariant);
    QCOMPARE(args[0].value<QVariant>(), QVariant { "third" });
}

} // namespace stylist
