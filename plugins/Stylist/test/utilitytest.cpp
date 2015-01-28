#include <QtTest/QtTest>

#include "utilitytest.h"

#include <util/url.h>

namespace stylist {

void UrlTest::
testConstructor()
{
    Url url1 { "http://www.google.com" };
    QVERIFY(url1.isValid());

    Url url2 { "random crap" };
    QVERIFY(url2.isRelative());

    //TODO invalid url
}

} // namespace stylist
