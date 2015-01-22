#include "stylesheettest.h"

#include "../stylesheet.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <QtTest/QtTest>

namespace stylist {
namespace css {

void StyleSheetTest::
init()
{
    namespace bfs = boost::filesystem;
    bfs::remove(m_path);
}

void StyleSheetTest::
cleanupTestCase()
{
    namespace bfs = boost::filesystem;
    bfs::remove(m_path);
}

void StyleSheetTest::
testConstructor()
{
    namespace bfs = boost::filesystem;

    bfs::ofstream ofs { m_path };

    ofs << "@-moz-document url(http://www.google.com),"
        << "               domain(google.co.jp)"
        << "{ body { background: yellow !important; } }";
    ofs.close();

    StyleSheet css { m_path };
}

void StyleSheetTest::
testHasStyleFor()
{
    namespace bfs = boost::filesystem;

    bfs::ofstream ofs { m_path };

    ofs << "@-moz-document url(http://www.google.com),"
        << "               domain(google.co.jp)"
        << "{ body { background: yellow !important; } }";
    ofs.close();

    StyleSheet css { m_path };

    QVERIFY(css.hasStyleFor("http://www.google.com"));
    QVERIFY(!css.hasStyleFor("http://www.facebook.com"));
}

void StyleSheetTest::
testStyleFor()
{
    namespace bfs = boost::filesystem;

    bfs::ofstream ofs { m_path };
    ofs << "@-moz-document url(http://www.google.com),\n"
        << "               domain(google.co.jp)\n"
        << "{ body { background: yellow !important; } }\n"
        << "\n"
        << "@-moz-document url-prefix(http://www.bing.com),\n"
        << "               domain(google.co.jp)\n"
        << "{ body { background: red !important; } }\n"
    ;
    ofs.close();

    StyleSheet css { m_path };

    const auto &shouldBeEmpty = css.styleFor("http://www.facebook.com");
    QVERIFY(shouldBeEmpty.empty());

    const auto &oneStyle = css.styleFor("http://www.google.com");
    QCOMPARE(oneStyle,
             std::string { " body { background: yellow !important; } " });

    const auto &twoStyles = css.styleFor("http://www.google.co.jp");
    QCOMPARE(twoStyles,
             std::string {
                " body { background: yellow !important; } \n"
                " body { background: red !important; } "
             });
}

} // namespace css
} // namespace stylist
