#include "../updater.hpp"

#include "core/settings.hpp"

#include <boost/filesystem.hpp>
#include <boost/range/iterator_range.hpp>

#include <QtNetwork/QNetworkAccessManager>
#include <QtTest/QSignalSpy>
#include <QtTest/QTest>

#include <utime.h>
#include <gtest/gtest.h>

static QNetworkAccessManager s_networkManager;

namespace adblock_dash {

TEST(Updater, NoSubscription)
{
    namespace bfs = boost::filesystem;

    const Path path { "data/updater/no_subscription" };

    bfs::remove_all(path);
    bfs::create_directory(path);

    Settings settings { path.c_str() };

    Updater updater { settings, s_networkManager };

    // At least non-intrusive exception rules will be updated.
    EXPECT_TRUE(updater());
}

TEST(Updater, NoExpiredSubscription)
{
    namespace bfs = boost::filesystem;

    const Path path { "data/updater/no_expired_subscription" };

    const auto &directory = boost::make_iterator_range(
        bfs::directory_iterator { path }, {}
    );
    for (const auto &entry: directory) {
        ASSERT_EQ(0, ::utime(entry.path().c_str(), nullptr)); // touch mtime
    }

    Settings settings { path.c_str() };

    Updater updater { settings, s_networkManager };

    EXPECT_FALSE(updater());
}

namespace bfs = boost::filesystem;

void rollBackTimeStamp(const bfs::path &path, const size_t days)
{
    ASSERT_TRUE(bfs::exists(path)) << path;

    auto &&mtime = bfs::last_write_time(path);
    mtime -= days * 24 * 60 * 60; // 10 days ago

    struct utimbuf times { mtime, mtime };
    const auto rc = ::utime(path.c_str(), &times);
    ASSERT_EQ(0, rc);
}

TEST(Updater, SuccessfulUpdate)
{
    rollBackTimeStamp("data/updater/successful_update/adblock_dash/first.txt", 10);
    rollBackTimeStamp("data/updater/successful_update/adblock_dash/second.txt", 10);

    Settings settings { "data/updater/successful_update" };

    Updater updater { settings, s_networkManager };
    QSignalSpy finished { &updater, SIGNAL(finished()) };
    ASSERT_TRUE(finished.isValid());
    QSignalSpy error { &updater, SIGNAL(error(const Subscription&, const QNetworkReply::NetworkError)) };
    ASSERT_TRUE(error.isValid());

    EXPECT_TRUE(updater());

    while (finished.isEmpty()) {
        QTest::qWait(250);
    }

    EXPECT_TRUE(error.empty());

    const auto &subscriptions = settings.subscriptions();

    EXPECT_TRUE(subscriptions[0].updateStatus());
    EXPECT_TRUE(subscriptions[1].updateStatus());
}

TEST(Updater, FailedUpdate)
{
    rollBackTimeStamp("data/updater/failed_update/adblock_dash/first.txt", 10);
    rollBackTimeStamp("data/updater/failed_update/adblock_dash/second.txt", 10);

    Settings settings { "data/updater/failed_update" };

    Updater updater { settings, s_networkManager };
    QSignalSpy finished { &updater, SIGNAL(finished()) };
    ASSERT_TRUE(finished.isValid());
    QSignalSpy error { &updater, SIGNAL(error(const Subscription&, const QNetworkReply::NetworkError)) };
    ASSERT_TRUE(error.isValid());

    EXPECT_TRUE(updater());

    while (finished.isEmpty()) {
        QTest::qWait(250);
    }

    EXPECT_EQ(1, error.size());
    const auto &arguments = error.front();
    ASSERT_EQ(2, arguments.size());

    const auto &subscriptions = settings.subscriptions();

    EXPECT_TRUE(subscriptions[0].updateStatus());
    EXPECT_FALSE(subscriptions[1].updateStatus());
}

TEST(Updater, SuccessfulRedirect)
{
    rollBackTimeStamp("data/updater/successful_redirect/adblock_dash/first.txt", 10);

    Settings settings { "data/updater/successful_redirect" };

    Updater updater { settings, s_networkManager };
    QSignalSpy finished { &updater, SIGNAL(finished()) };
    ASSERT_TRUE(finished.isValid());
    QSignalSpy error { &updater, SIGNAL(error(const Subscription&, const QNetworkReply::NetworkError)) };
    ASSERT_TRUE(error.isValid());

    EXPECT_TRUE(updater());

    while (finished.isEmpty()) {
        QTest::qWait(250);
    }

    EXPECT_TRUE(error.empty());

    const auto &subscriptions = settings.subscriptions();

    EXPECT_TRUE(subscriptions[0].updateStatus());
}

} // namespace adblock_dash
