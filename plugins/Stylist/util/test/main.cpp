#include "signalbuffertest.h"
#include "directorywatchertest.h"

#include <QtGui/QApplication>
#include <QtTest/QtTest>

int main(int argc, char*argv[])
{
    QApplication app(argc, argv);

    stylist::SignalBufferTest signalBufferTest;
    QTest::qExec(&signalBufferTest, argc, argv);

    stylist::DirectoryWatcherTest directoryWatcherTest;
    QTest::qExec(&directoryWatcherTest, argc, argv);
}
