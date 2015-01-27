#include "signalbuffertest.h"

#include "../signalbuffer.h"

#include <QtCore/QDebug>
#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>

namespace stylist {

void SignalBufferTest::
testConstructor()
{
    SignalBuffer buffer { *this, SIGNAL(fire(const QString&)) };
}

void SignalBufferTest::
testBuffering()
{
    SignalBuffer buffer { *this, SIGNAL(fire(const QString&)) };
    QSignalSpy spy { &buffer, SIGNAL(triggered(const QString&)) };

    Q_EMIT fire("first");

    QTest::qWait(100);
    QCOMPARE(spy.size(), 0);

    QTest::qWait(300);
    QCOMPARE(spy.size(), 1);
    const auto args = spy.takeFirst();
    QCOMPARE(args.size(), 1);
    QCOMPARE(args[0].type(), QVariant::String);
    QCOMPARE(args[0].toString(), QString { "first" });
}

void SignalBufferTest::
testBuffering2()
{
    SignalBuffer buffer { *this, SIGNAL(fire(const QString&)) };
    QSignalSpy spy { &buffer, SIGNAL(triggered(const QString&)) };

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
    QCOMPARE(args[0].type(), QVariant::String);
    QCOMPARE(args[0].toString(), QString { "first" });
}

void SignalBufferTest::
testBuffering3()
{
    SignalBuffer buffer { *this, SIGNAL(fire(const QString&)) };
    QSignalSpy spy { &buffer, SIGNAL(triggered(const QString&)) };

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
    QCOMPARE(args[0].type(), QVariant::String);
    QCOMPARE(args[0].toString(), QString { "first" });

    args = spy.takeFirst();
    QCOMPARE(args.size(), 1);
    QCOMPARE(args[0].type(), QVariant::String);
    QCOMPARE(args[0].toString(), QString { "second" });

    args = spy.takeFirst();
    QCOMPARE(args.size(), 1);
    QCOMPARE(args[0].type(), QVariant::String);
    QCOMPARE(args[0].toString(), QString { "third" });
}

} // namespace stylist
