#include "signalbuffertest.h"
#include "directorywatchertest.h"
#include "buffereddirectorywatchertest.h"
#include "styletest.h"

#include <QtGui/QApplication>
#include <QtTest/QtTest>

int main(int argc, char*argv[])
{
    QApplication app(argc, argv);

    stylist::SignalBufferTest signalBufferTest;
    QTest::qExec(&signalBufferTest, argc, argv);

    stylist::DirectoryWatcherTest directoryWatcherTest;
    QTest::qExec(&directoryWatcherTest, argc, argv);

    stylist::BufferedDirectoryWatcherTest bufferedDirectoryWatcherTest;
    QTest::qExec(&bufferedDirectoryWatcherTest, argc, argv);

    stylist::StylesTest stylesTest;
    QTest::qExec(&stylesTest, argc, argv);
}
