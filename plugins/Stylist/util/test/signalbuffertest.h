#ifndef STYLIST_SIGNALBUFFER_TEST_H
#define STYLIST_SIGNALBUFFER_TEST_H

namespace stylist {

class SignalBufferTest : public QObject
{
    Q_OBJECT

private Q_SLOTS:
    void testConstructor();
    void testBuffering();
    void testBuffering2();
    void testBuffering3();

Q_SIGNALS:
    void fire(const QString &);
};

} // namespace stylist

#endif // STYLIST_SIGNALBUFFER_TEST_H
