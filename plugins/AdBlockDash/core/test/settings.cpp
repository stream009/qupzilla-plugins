#include "../settings.hpp"

#include "common/test.hpp"

#include <iostream>

#include <gtest/gtest.h>

namespace adblock_dash {

TEST(Settings, exceptionRules_default)
{
    Settings settings { "data/settings" };

    const auto &subscription = settings.exceptionRules();
    EXPECT_EQ("Allow non-intrusive advertising", subscription.name());
    EXPECT_STREQ("data/settings/adblock_dash/exceptionrules.txt",
                 subscription.path().c_str());
    EXPECT_EQ("https://easylist-downloads.adblockplus.org/exceptionrules.txt",
              subscription.location().toString());
    EXPECT_TRUE(subscription.enabled());
}

} // namespace adblock_dash
