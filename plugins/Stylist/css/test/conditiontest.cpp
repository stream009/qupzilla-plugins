#include "conditiontest.h"

#include "../condition.h"

#include <algorithm>

#include <boost/range/iterator_range.hpp>
#include <boost/utility/string_ref.hpp>

#include <QtTest/QtTest>

namespace stylist {
namespace css {

void ConditionTest::
testUrlCondition()
{
    std::vector<std::unique_ptr<Condition>> conditions;
    const auto &text = boost::make_iterator_range(
        "@-moz-document url(http://www.bing.com),"
        "               url(http://www.facebook.com)"
        "{ }"
    );

    UrlCondition::extract(text, std::back_inserter(conditions));

    QCOMPARE(conditions.size(), 2ul);

    QVERIFY(std::any_of(
        conditions.begin(), conditions.end(),
        [] (const std::unique_ptr<Condition> &cond) {
            return cond->match("http://www.bing.com");
        })
    );

    QVERIFY(std::any_of(
        conditions.begin(), conditions.end(),
        [] (const std::unique_ptr<Condition> &cond) {
            return cond->match("http://www.facebook.com");
        })
    );

    QVERIFY(std::none_of(conditions.begin(), conditions.end(),
        [] (const std::unique_ptr<Condition> &cond) {
            return cond->match("http://www.google.com");
        })
    );
}

void ConditionTest::
testUrlPrefixCondition()
{
    const auto &text = boost::make_iterator_range(
        "url-prefix(http://www.bing.com),"
        "url-prefix(\"http://www.facebook.com/foo\"),"
        "{ }"
    );
    std::vector<std::unique_ptr<Condition>> conditions;

    UrlPrefixCondition::extract(text, std::back_inserter(conditions));

    QCOMPARE(conditions.size(), 2ul);

    QVERIFY(std::any_of(
        conditions.begin(), conditions.end(),
        [] (const std::unique_ptr<Condition> &cond) {
            return cond->match("http://www.bing.com/search");
        })
    );

    QVERIFY(std::none_of(
        conditions.begin(), conditions.end(),
        [] (const std::unique_ptr<Condition> &cond) {
            return cond->match("http://www.facebook.com/bar");
        })
    );
}

void ConditionTest::
testDomainCondition()
{
    const auto &text = boost::make_iterator_range(
        "domain(google.com),"
        "domain(\"facebook.com\"),"
        "{ }"
    );
    std::vector<std::unique_ptr<Condition>> conditions;

    DomainCondition::extract(text, std::back_inserter(conditions));

    QCOMPARE(conditions.size(), 2ul);

    QVERIFY(std::any_of(
        conditions.begin(), conditions.end(),
        [] (const std::unique_ptr<Condition> &cond) {
            return cond->match("http://www.google.com/search");
        })
    );

    QVERIFY(std::none_of(
        conditions.begin(), conditions.end(),
        [] (const std::unique_ptr<Condition> &cond) {
            return cond->match("http://www.bing.com/bar");
        })
    );
}

void ConditionTest::
testRegExCondition()
{
    const auto &text = boost::make_iterator_range(
        R"(regex(".*\.com"),)"
        R"(regex('.*\.google\.com.*'),)"
        "{ }"
    );
    std::vector<std::unique_ptr<Condition>> conditions;

    RegExCondition::extract(text, std::back_inserter(conditions));

    QCOMPARE(conditions.size(), 2ul);

    QVERIFY(std::any_of(
        conditions.begin(), conditions.end(),
        [] (const std::unique_ptr<Condition> &cond) {
            return cond->match("http://mail.google.com");
        })
    );

    QVERIFY(std::none_of(
        conditions.begin(), conditions.end(),
        [] (const std::unique_ptr<Condition> &cond) {
            return cond->match("http://www.whitehouse.gov");
        })
    );
}

}} // namespace stylist::css
