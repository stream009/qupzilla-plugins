#include "../subscription.hpp"

#include <utime.h>

#include <gtest/gtest.h>

namespace adblock_dash {

TEST(Subscription, integrityCheck)
{
    const QUrl url { "http://www.google.com" };

    Subscription foo (
        "EasyList",
        "data/easylist.txt",
        url
    );
    EXPECT_TRUE(foo.hasIntegrity());

    Subscription bar (
        "EasyList",
        "data/checksum_error.txt",
        url
    );
    EXPECT_FALSE(bar.hasIntegrity());
}

TEST(Subscription, isExpired)
{
    const QUrl url { "http://www.google.com" };

    Subscription foo (
        "EasyList",
        "data/easylist.txt",
        url
    );
    EXPECT_TRUE(foo.isExpired());

    const auto* const path = "data/checksum_error.txt";
    ASSERT_EQ(0, utime(path, nullptr)); // touch mtime

    Subscription bar (
        "EasyList",
        path,
        url
    );
    EXPECT_FALSE(bar.isExpired());

    const auto &random_path = boost::filesystem::unique_path();
    Subscription subscription3 {
        "File doesn't exist",
        random_path,
        url
    };
    EXPECT_TRUE(subscription3.isExpired());
}

} // namespace adblock_dash
