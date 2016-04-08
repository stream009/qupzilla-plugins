#include "../settings.hpp"

#include <boost/filesystem.hpp>

#include <gtest/gtest.h>

using namespace adblock_dash;

TEST(Settings, Constructor)
{
    Settings settings { "test.ini" };
}

const char*
prepareTestFile()
{
    namespace bfs = boost::filesystem;
    static const char filename[] = "test.ini";
    bfs::remove(filename);
    assert(!bfs::exists(filename));

    return filename;
}

TEST(Settings, AppendSubscription)
{
    const auto &filename = prepareTestFile();

    Settings settings { filename };

    auto &&subscriptions = settings.subscriptions();
    ASSERT_EQ(0, subscriptions.size());

    subscriptions.append("foo", "/somewhere/over/the/rainbow");
    ASSERT_EQ(1, subscriptions.size());

    const auto &entry = subscriptions[0];
    EXPECT_EQ("foo", entry.name);
    EXPECT_EQ("/somewhere/over/the/rainbow", entry.path);
}

TEST(Settings, RemoveSubscription)
{
    const auto &filename = prepareTestFile();

    Settings settings { filename };

    auto &&subscriptions = settings.subscriptions();
    ASSERT_EQ(0, subscriptions.size());

    subscriptions.append("foo", "/somewhere/over/the/rainbow");
    ASSERT_EQ(1, subscriptions.size());

    subscriptions.append("bar", "/no/place/like/home");
    ASSERT_EQ(2, subscriptions.size());

    subscriptions.remove("/somewhere/over/the/rainbow");
    ASSERT_EQ(1, subscriptions.size());

    const auto &entry = subscriptions[0];
    EXPECT_EQ("bar", entry.name);
    EXPECT_EQ("/no/place/like/home", entry.path);
}

#include <QtCore/QObject>

struct Watcher : public QObject
{
    Q_OBJECT
public Q_SLOTS:
    void onAppended(const QString&, const QString&)
    {
        m_appendedSignal = true;
    }

    void onRemoved(const QString&)
    {
        m_removedSignal = true;
    }
public:
    bool m_appendedSignal = false;
    bool m_removedSignal = false;
};

#include "settings_test.moc"

TEST(Settings, SubscriptionSignal)
{
    Watcher watcher;
    const auto &filename = prepareTestFile();

    Settings settings { filename };

    auto &&subscriptions = settings.subscriptions();
    ASSERT_EQ(0, subscriptions.size());

    QObject::connect(
        &subscriptions, SIGNAL(appended(const QString&, const QString&)),
        &watcher,         SLOT(onAppended(const QString&, const QString&)));
    QObject::connect(
        &subscriptions, SIGNAL(removed(const QString&)),
        &watcher,         SLOT(onRemoved(const QString&)));

    ASSERT_FALSE(watcher.m_appendedSignal);
    ASSERT_FALSE(watcher.m_removedSignal);

    subscriptions.append("foo", "/somewhere/over/the/rainbow");
    ASSERT_EQ(1, subscriptions.size());
    EXPECT_TRUE(watcher.m_appendedSignal);
    EXPECT_FALSE(watcher.m_removedSignal);

    subscriptions.remove("/somewhere/over/the/rainbow");
    ASSERT_EQ(0, subscriptions.size());
    EXPECT_TRUE(watcher.m_removedSignal);
}

TEST(Settings, Directory)
{
    const auto &filename = prepareTestFile();
    Settings settings { filename };

    settings.directory = "foo";

    EXPECT_EQ("foo", settings.directory);
}
