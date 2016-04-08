#include "conditiontest.h"
#include "documentruletest.h"
#include "stylesheettest.h"

#include <QtWidgets/QApplication>
#include <QtTest/QtTest>

int main(int argc, char*argv[])
{
    using namespace stylist::css;

    QApplication app(argc, argv);

    ConditionTest conditionTest;
    QTest::qExec(&conditionTest, argc, argv);

    DocumentRuleTest documentRuleTest;
    QTest::qExec(&documentRuleTest, argc, argv);

    StyleSheetTest styleSheetTest;
    QTest::qExec(&styleSheetTest, argc, argv);
}
