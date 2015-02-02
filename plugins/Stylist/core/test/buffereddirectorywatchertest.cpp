#include "buffereddirectorywatchertest.h"

#include "../buffereddirectorywatcher.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <QtTest/QSignalSpy>

namespace stylist {

using Path = boost::filesystem::path;
const Path BufferedDirectoryWatcherTest::m_dir { "target_dir" };

void BufferedDirectoryWatcherTest::
init()
{
    namespace bfs = boost::filesystem;

    bfs::create_directory(m_dir);
}

void BufferedDirectoryWatcherTest::
cleanup()
{
    namespace bfs = boost::filesystem;
    bfs::remove_all(m_dir);
}

void BufferedDirectoryWatcherTest::
testConstructor()
{
    BufferedDirectoryWatcher watcher { m_dir };
}

void BufferedDirectoryWatcherTest::
testFileAdded()
{
    BufferedDirectoryWatcher watcher { m_dir };
    QSignalSpy addedSpy { &watcher, SIGNAL(fileAdded(const Path&)) };
    QSignalSpy deletedSpy { &watcher, SIGNAL(fileDeleted(const Path&)) };
    QSignalSpy modifiedSpy { &watcher, SIGNAL(fileModified(const Path&)) };

    QCOMPARE(addedSpy.size(), 0);
    QCOMPARE(deletedSpy.size(), 0);
    QCOMPARE(modifiedSpy.size(), 0);

    namespace bfs = boost::filesystem;

    const auto &filePath = m_dir / "foo";
    bfs::ofstream file { m_dir / "foo" };
    file.close();

    QTest::qWait(100);
    QCOMPARE(addedSpy.size(), 0);
    QCOMPARE(deletedSpy.size(), 0);
    QCOMPARE(modifiedSpy.size(), 0);

    QTest::qWait(400);

    QCOMPARE(addedSpy.size(), 1);
    QCOMPARE(deletedSpy.size(), 0);
    QCOMPARE(modifiedSpy.size(), 0);
    const auto &args = addedSpy.takeFirst();
    QCOMPARE(args.size(), 1);
    QCOMPARE(args[0].typeName(), "boost::filesystem::path");
    QCOMPARE(args[0].value<Path>(), filePath);
}

void BufferedDirectoryWatcherTest::
testFileDeleted()
{
    namespace bfs = boost::filesystem;

    const auto &filePath = m_dir / "bar";
    bfs::ofstream file { filePath };
    file.close();

    BufferedDirectoryWatcher watcher { m_dir };
    QSignalSpy addedSpy { &watcher, SIGNAL(fileAdded(const Path&)) };
    QSignalSpy deletedSpy { &watcher, SIGNAL(fileDeleted(const Path&)) };
    QSignalSpy modifiedSpy { &watcher, SIGNAL(fileModified(const Path&)) };

    QCOMPARE(addedSpy.size(), 0);
    QCOMPARE(deletedSpy.size(), 0);
    QCOMPARE(modifiedSpy.size(), 0);

    bfs::remove(filePath);

    QTest::qWait(100);
    QCOMPARE(addedSpy.size(), 0);
    QCOMPARE(deletedSpy.size(), 0);
    QCOMPARE(modifiedSpy.size(), 0);

    QTest::qWait(400);
    QCOMPARE(addedSpy.size(), 0);
    QCOMPARE(deletedSpy.size(), 1);
    QCOMPARE(modifiedSpy.size(), 0);

    const auto &args = deletedSpy.takeFirst();
    QCOMPARE(args.size(), 1);
    QCOMPARE(args[0].typeName(), "boost::filesystem::path");
    QCOMPARE(args[0].value<Path>(), filePath);
}

void BufferedDirectoryWatcherTest::
testFileModified()
{
    namespace bfs = boost::filesystem;

    const auto &filePath = m_dir / "bar";
    bfs::ofstream file { filePath };
    file.close();

    BufferedDirectoryWatcher watcher { m_dir };
    QSignalSpy addedSpy { &watcher, SIGNAL(fileAdded(const Path&)) };
    QSignalSpy deletedSpy { &watcher, SIGNAL(fileDeleted(const Path&)) };
    QSignalSpy modifiedSpy { &watcher, SIGNAL(fileModified(const Path&)) };

    QCOMPARE(addedSpy.size(), 0);
    QCOMPARE(deletedSpy.size(), 0);
    QCOMPARE(modifiedSpy.size(), 0);

    file.open(filePath);
    file << "change change";
    file.close();

    QTest::qWait(100);
    QCOMPARE(addedSpy.size(), 0);
    QCOMPARE(deletedSpy.size(), 0);
    QCOMPARE(modifiedSpy.size(), 0);

    QTest::qWait(400);
    QCOMPARE(addedSpy.size(), 0);
    QCOMPARE(deletedSpy.size(), 0);
    QCOMPARE(modifiedSpy.size(), 1);

    const auto &args = modifiedSpy.takeFirst();
    QCOMPARE(args.size(), 1);
    QCOMPARE(args[0].typeName(), "boost::filesystem::path");
    QCOMPARE(args[0].value<Path>(), filePath);
}

} // namespace stylist
