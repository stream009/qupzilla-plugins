#include "styletest.h"

#include <QtGui/QApplication>
#include <QtTest/QtTest>

int main(int argc, char*argv[])
{
    using namespace stylist;

    QApplication app(argc, argv);

    StylesTest stylesTest;
    QTest::qExec(&stylesTest, argc, argv);
}
