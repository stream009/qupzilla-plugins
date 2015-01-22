#include "documentruletest.h"

#include "../documentrule.h"

#include <boost/algorithm/string/trim.hpp>

#include <QtTest/QtTest>

namespace stylist {
namespace css {

void DocumentRuleTest::
testExtractZeroRule()
{
    const char text[] = "";

    std::vector<DocumentRule> rules;
    DocumentRule::extract(text, std::back_inserter(rules));

    QVERIFY(rules.empty());
}

void DocumentRuleTest::
testExtractOneRule()
{
    const char text[] =
        "@-moz-document url(http://www.bing.com),"
        "               url-prefix(http://www.facebook.com),"
        "               domain(google.com)"
        R"(             regex(".*/search/.*"))"
        "{ background: yellow }";

    std::vector<DocumentRule> rules;
    DocumentRule::extract(text, std::back_inserter(rules));

    QCOMPARE(rules.size(), 1u);
    QCOMPARE(boost::algorithm::trim_copy(rules[0].body()),
             std::string { "background: yellow" });
    QVERIFY(rules[0].match("http://www.bing.com"));
    QVERIFY(!rules[0].match("http://www.google.co.uk"));
}

void DocumentRuleTest::
testExtractMultipleRules()
{
    const char text[] =
        "@-moz-document url(http://www.bing.com),"
        "               domain(google.com)"
        "{ background: yellow }"
        ""
        "@-moz-document url-prefix(http://www.facebook.com/foo),"
        R"(             regex(".*/search/.*"))"
        "{ background: red }";

    std::vector<DocumentRule> rules;
    DocumentRule::extract(text, std::back_inserter(rules));

    QCOMPARE(rules.size(), 2u);
    QCOMPARE(boost::algorithm::trim_copy(rules[0].body()),
             std::string { "background: yellow" });
    QCOMPARE(boost::algorithm::trim_copy(rules[1].body()),
             std::string { "background: red" });
    QVERIFY(rules[0].match("http://www.bing.com"));
    QVERIFY(!rules[0].match("http://mail.google.co.uk/somebody"));
    QVERIFY(rules[1].match("http://www.google.com/search/keyword"));
    QVERIFY(!rules[1].match("http://www.facebook.com/bar"));
}

} // namespace stylist
} // namespace css
