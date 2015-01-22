#include "styletest.h"

#include "../utility.h"
#include "../styles.h"

#include <fstream>

#include <boost/algorithm/string/trim.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/range/iterator_range.hpp>

#include <QtTest/QtTest>

namespace stylist {

namespace bfs = boost::filesystem;

void StylesTest::
initTestCase()
{
    if (bfs::exists(m_directory)) {
        bfs::remove_all(m_directory);
    }

    bfs::create_directory(m_directory);
    QVERIFY(bfs::exists(m_directory) &&
            bfs::is_directory(m_directory));
}

void StylesTest::
cleanupTestCase()
{
    bfs::remove_all(m_directory);
}

void StylesTest::
init()
{
    QVERIFY(bfs::exists(m_directory) &&
            bfs::is_directory(m_directory));

    bfs::directory_iterator it { m_directory };
    for (const auto &entry: boost::make_iterator_range(it, {})) {
        bfs::remove(entry.path());
    }
}

void StylesTest::
cleanup()
{
}

void StylesTest::
testConstructor()
{
    bfs::ofstream ofs { m_directory / "test.css" };
    ofs << "@-moz-document url(http://www.google.com),"
        << "               domain(google.co.jp)"
        << "{ body { background: yellow !important; } }";
    ofs.close();

    Styles styles { m_directory };
}

template<typename Path, typename Contents>
static void
createFile(const Path &path, const Contents &contents)
{
    bfs::ofstream ofs { path };
    ofs << contents;
    ofs.close();
}

void StylesTest::
testQuery()
{
    namespace ba = boost::algorithm;
    Styles styles0 { m_directory };
    QVERIFY(styles0.query("http://www.google.com").empty());

    const char contents1[] =
        "@-moz-document url(http://www.google.com),"
        "               domain(google.co.jp)"
        "{ body { background: yellow !important; } }";
    createFile(m_directory / "test1.css", contents1);

    Styles styles1 { m_directory };
    QCOMPARE(styles1.query("http://www.google.com"),
             std::string { " body { background: yellow !important; } " });

    const char contents2[] =
        "@-moz-document url(http://www.google.com),"
        "               domain(google.co.jp)"
        "{ body { background: red !important; } }";
    createFile(m_directory / "test2.css", contents2);

    Styles styles2 { m_directory };
    QCOMPARE(styles2.query("http://www.google.com"),
             std::string {
                " body { background: yellow !important; } \n"
                " body { background: red !important; } "
             });
}

} // namespace stylist
