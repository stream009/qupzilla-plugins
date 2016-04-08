#include <ostream>

#include <QtCore/QObject>
#include <QtCore/QString>
#include <QtCore/QTimer>
#include <QtWidgets/QApplication>

#include <gtest/gtest.h>

class TestMain : public QObject
{
    Q_OBJECT
private Q_SLOTS:
    void run()
    {
        const auto rv = RUN_ALL_TESTS();
        qApp->exit(rv);
    }
};


int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    QApplication app(argc, argv);

    TestMain test;
    QTimer::singleShot(0, &test, SLOT(run()));

    return app.exec();
}

#include "gtest_qt_main.moc"
