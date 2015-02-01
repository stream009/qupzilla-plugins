#include "styletest.h"

#include "utility.h"
#include "styles.h"
#include "serialization/styles.h"

#include <fstream>

#include <boost/algorithm/string/trim.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/range/iterator_range.hpp>

#include <QtTest/QtTest>
#include <QtTest/QSignalSpy>

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
testConstructor() const
{
    bfs::ofstream ofs { m_directory / "test.css" };
    ofs << "@-moz-document url(http://www.google.com),"
        << "               domain(google.co.jp)"
        << "{ body { background: yellow !important; } }";
    ofs.close();

    Styles styles { m_directory };
    styles.scanDirectory();

    QCOMPARE(styles.size(), 1u);
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
testQuery() const
{
    Styles styles0 { m_directory };
    QVERIFY(styles0.query("http://www.google.com").empty());

    const char contents1[] =
        "@-moz-document url(http://www.google.com),"
        "               domain(google.co.jp)"
        "{ body { background: yellow !important; } }";
    createFile(m_directory / "test1.css", contents1);

    Styles styles1 { m_directory };
    styles1.scanDirectory();
    QCOMPARE(styles1.query("http://www.google.com"),
             std::string { " body { background: yellow !important; } " });

    const char contents2[] =
        "@-moz-document url(http://www.google.com),"
        "               domain(google.co.jp)"
        "{ body { background: red !important; } }";
    createFile(m_directory / "test2.css", contents2);

    Styles styles2 { m_directory };
    styles2.scanDirectory();
    QCOMPARE(styles2.query("http://www.google.com"),
             std::string {
                " body { background: yellow !important; } \n"
                " body { background: red !important; } "
             });
}

void StylesTest::
testAddFile() const
{
    Styles styles { m_directory };
    QVERIFY(styles.empty());

    QSignalSpy spy { &styles, SIGNAL(changed()) };
    QVERIFY(spy.isEmpty());

    const char contents[] =
        "@-moz-document url(http://www.google.com),"
        "               domain(google.co.jp)"
        "{ body { background: yellow !important; } }";
    createFile(m_directory / "test.css", contents);

    QTest::qWait(400);
    QCOMPARE(spy.size(), 1);
    const auto &args = spy.takeFirst();
    QVERIFY(args.isEmpty());

    QCOMPARE(styles.size(), 1u);
}

void StylesTest::
testDeleteFile() const
{
    Styles styles { m_directory };

    const char contents[] =
        "@-moz-document url(http://www.google.com),"
        "               domain(google.co.jp)"
        "{ body { background: yellow !important; } }";
    const auto &path = m_directory / "test.css";
    createFile(path, contents);

    QTest::qWait(400);

    QSignalSpy spy { &styles, SIGNAL(changed()) };
    QVERIFY(spy.isEmpty());
    QCOMPARE(styles.size(), 1u);

    bfs::remove(path);
    QTest::qWait(400);

    QCOMPARE(spy.size(), 1);
    const auto &args = spy.takeFirst();
    QVERIFY(args.isEmpty());

    QVERIFY(styles.empty());
}

void StylesTest::
testModifyFile() const
{
    Styles styles { m_directory };

    const char contents[] =
        "@-moz-document url(http://www.google.com),"
        "               domain(google.co.jp)"
        "{ body { background: yellow !important; } }";
    const auto &path = m_directory / "test.css";
    createFile(path, contents);

    QTest::qWait(400);

    QSignalSpy spy { &styles, SIGNAL(changed()) };
    QVERIFY(spy.isEmpty());
    QCOMPARE(styles.size(), 1u);

    bfs::fstream file { path };
    const char addition[] =
        "@-moz-document url(http://www.bing.com)"
        "{ body { background: red !important; } }";
    file << addition;
    file.close();
    QTest::qWait(400);

    QCOMPARE(spy.size(), 1);
    const auto &args = spy.takeFirst();
    QVERIFY(args.isEmpty());

    QCOMPARE(styles.size(), 1u);
}

void StylesTest::
testRenameFile() const
{
    Styles styles { m_directory };

    const char contents[] =
        "@-moz-document url(http://www.google.com),"
        "               domain(google.co.jp)"
        "{ body { background: yellow !important; } }";
    const auto &path = m_directory / "test.css";
    createFile(path, contents);

    QTest::qWait(400);

    QSignalSpy spy { &styles, SIGNAL(changed()) };
    QVERIFY(spy.isEmpty());
    QCOMPARE(styles.size(), 1u);

    bfs::rename(path, m_directory / "renamed.css");
    QTest::qWait(400);

    QCOMPARE(spy.size(), 2); // 1 delete, 1 add
    auto args = spy.takeFirst();
    QVERIFY(args.isEmpty());
    args = spy.takeFirst();
    QVERIFY(args.isEmpty());

    QCOMPARE(styles.size(), 1u);
}

void StylesTest::
testSerialize() const
{
    const char contents1[] =
        "@-moz-document url(http://www.google.com),"
        "               domain(google.co.jp)"
        "{ body { background: yellow !important; } }";
    const auto &path1 = m_directory / "test1.css";
    createFile(path1, contents1);

    Styles styles { m_directory };
    styles.scanDirectory();
    QCOMPARE(styles.size(), 1u);

    std::stringstream ss;
    boost::archive::text_oarchive dat { ss };

    Styles *ptr = &styles;
    dat << ptr;

    //qDebug() << ss.str().c_str();

    boost::archive::text_iarchive iar { ss };
    ptr = nullptr;
    iar >> ptr;
    QVERIFY(ptr != &styles);

    QCOMPARE(ptr->size(), 1u);
    QCOMPARE(ptr->m_directory, m_directory);

    const auto &first = ptr->at(0);
    QCOMPARE(&first.m_parent, ptr);
    QCOMPARE(first.name(), styles.at(0).name());
    QCOMPARE(first.path(), styles.at(0).path());
    QCOMPARE(first.enabled(), styles.at(0).enabled());
}

} // namespace stylist
