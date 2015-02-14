#include "directorywatchertest.h"

#include "../directorywatcher.h"

#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>

#include <QtTest/QSignalSpy>

namespace stylist {

using Path = boost::filesystem::path;
const Path DirectoryWatcherTest::m_dir { "target_dir" };

void DirectoryWatcherTest::
init()
{
    namespace bfs = boost::filesystem;

    bfs::create_directory(m_dir);
}

void DirectoryWatcherTest::
cleanup()
{
    namespace bfs = boost::filesystem;
    bfs::remove_all(m_dir);
}

void DirectoryWatcherTest::
testConstructor()
{
    DirectoryWatcher watcher { m_dir };
}

void DirectoryWatcherTest::
testFileAdded()
{
    DirectoryWatcher watcher { m_dir };
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

    QCOMPARE(addedSpy.size(), 1);
    QCOMPARE(deletedSpy.size(), 0);
    QCOMPARE(modifiedSpy.size(), 0);
    const auto &args = addedSpy.takeFirst();
    QCOMPARE(args.size(), 1);
    QCOMPARE(args[0].typeName(), "boost::filesystem::path");
    QCOMPARE(args[0].value<Path>(), filePath);
}

void DirectoryWatcherTest::
testMultipleFilesAdded()
{
    DirectoryWatcher watcher { m_dir };
    QSignalSpy addedSpy { &watcher, SIGNAL(fileAdded(const Path&)) };
    QSignalSpy deletedSpy { &watcher, SIGNAL(fileDeleted(const Path&)) };
    QSignalSpy modifiedSpy { &watcher, SIGNAL(fileModified(const Path&)) };

    QCOMPARE(addedSpy.size(), 0);
    QCOMPARE(deletedSpy.size(), 0);
    QCOMPARE(modifiedSpy.size(), 0);

    namespace bfs = boost::filesystem;

    const auto &filePath1 = m_dir / "bar1";
    bfs::ofstream file { m_dir / "bar1" };
    file.close();

    const auto &filePath2 = m_dir / "bar2";
    file.open(m_dir / "bar2");
    file.close();

    QTest::qWait(100);

    QCOMPARE(addedSpy.size(), 2);
    QCOMPARE(deletedSpy.size(), 0);
    QCOMPARE(modifiedSpy.size(), 0);

    auto &&args = addedSpy.takeFirst();
    QCOMPARE(args.size(), 1);
    QCOMPARE(args[0].typeName(), "boost::filesystem::path");
    QCOMPARE(args[0].value<Path>(), filePath1);

    args = addedSpy.takeFirst();
    QCOMPARE(args.size(), 1);
    QCOMPARE(args[0].typeName(), "boost::filesystem::path");
    QCOMPARE(args[0].value<Path>(), filePath2);
}

void DirectoryWatcherTest::
testFileDeleted()
{
    namespace bfs = boost::filesystem;

    const auto &filePath = m_dir / "bar";
    bfs::ofstream file { filePath };
    file.close();

    DirectoryWatcher watcher { m_dir };
    QSignalSpy addedSpy { &watcher, SIGNAL(fileAdded(const Path&)) };
    QSignalSpy deletedSpy { &watcher, SIGNAL(fileDeleted(const Path&)) };
    QSignalSpy modifiedSpy { &watcher, SIGNAL(fileModified(const Path&)) };

    QCOMPARE(addedSpy.size(), 0);
    QCOMPARE(deletedSpy.size(), 0);
    QCOMPARE(modifiedSpy.size(), 0);

    bfs::remove(filePath);
    QTest::qWait(100);

    QCOMPARE(addedSpy.size(), 0);
    QCOMPARE(deletedSpy.size(), 1);
    QCOMPARE(modifiedSpy.size(), 0);

    const auto &args = deletedSpy.takeFirst();
    QCOMPARE(args.size(), 1);
    QCOMPARE(args[0].typeName(), "boost::filesystem::path");
    QCOMPARE(args[0].value<Path>(), filePath);
}

void DirectoryWatcherTest::
testMultipleFilesDeleted()
{
    namespace bfs = boost::filesystem;

    const auto &filePath1 = m_dir / "bar1";
    bfs::ofstream file1 { filePath1 };
    file1.close();

    const auto &filePath2 = m_dir / "bar2";
    bfs::ofstream file2 { filePath2 };
    file2.close();

    DirectoryWatcher watcher { m_dir };
    QSignalSpy addedSpy { &watcher, SIGNAL(fileAdded(const Path&)) };
    QSignalSpy deletedSpy { &watcher, SIGNAL(fileDeleted(const Path&)) };
    QSignalSpy modifiedSpy { &watcher, SIGNAL(fileModified(const Path&)) };

    QCOMPARE(addedSpy.size(), 0);
    QCOMPARE(deletedSpy.size(), 0);
    QCOMPARE(modifiedSpy.size(), 0);

    bfs::remove(filePath1);
    bfs::remove(filePath2);
    QTest::qWait(100);

    QCOMPARE(addedSpy.size(), 0);
    QCOMPARE(deletedSpy.size(), 2);
    QCOMPARE(modifiedSpy.size(), 0);

    auto &&args = deletedSpy.takeFirst();
    QCOMPARE(args.size(), 1);
    QCOMPARE(args[0].typeName(), "boost::filesystem::path");
    QCOMPARE(args[0].value<Path>(), filePath1);

    args = deletedSpy.takeFirst();
    QCOMPARE(args.size(), 1);
    QCOMPARE(args[0].typeName(), "boost::filesystem::path");
    QCOMPARE(args[0].value<Path>(), filePath2);
}

void DirectoryWatcherTest::
testFileRenamed()
{
    namespace bfs = boost::filesystem;

    const auto &filePath = m_dir / "bar";
    bfs::ofstream file { filePath };
    file.close();

    DirectoryWatcher watcher { m_dir };
    QSignalSpy addedSpy { &watcher, SIGNAL(fileAdded(const Path&)) };
    QSignalSpy deletedSpy { &watcher, SIGNAL(fileDeleted(const Path&)) };
    QSignalSpy modifiedSpy { &watcher, SIGNAL(fileModified(const Path&)) };

    QCOMPARE(addedSpy.size(), 0);
    QCOMPARE(deletedSpy.size(), 0);
    QCOMPARE(modifiedSpy.size(), 0);

    const auto &renameTo = m_dir / "foo";
    bfs::rename(filePath, renameTo);
    QTest::qWait(100);

    QCOMPARE(addedSpy.size(), 1);
    QCOMPARE(deletedSpy.size(), 1);
    QCOMPARE(modifiedSpy.size(), 0);

    auto &&args = deletedSpy.takeFirst();
    QCOMPARE(args.size(), 1);
    QCOMPARE(args[0].typeName(), "boost::filesystem::path");
    QCOMPARE(args[0].value<Path>(), filePath);

    args = addedSpy.takeFirst();
    QCOMPARE(args.size(), 1);
    QCOMPARE(args[0].typeName(), "boost::filesystem::path");
    QCOMPARE(args[0].value<Path>(), renameTo);
}

void DirectoryWatcherTest::
testFileModified()
{
    namespace bfs = boost::filesystem;

    const auto &filePath = m_dir / "bar";
    bfs::ofstream file { filePath };
    file.close();

    DirectoryWatcher watcher { m_dir };
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
    QCOMPARE(modifiedSpy.size(), 1);

    const auto &args = modifiedSpy.takeFirst();
    QCOMPARE(args.size(), 1);
    QCOMPARE(args[0].typeName(), "boost::filesystem::path");
    QCOMPARE(args[0].value<Path>(), filePath);
}

void DirectoryWatcherTest::
testMultipleFilesModified()
{
    namespace bfs = boost::filesystem;

    const auto &filePath1 = m_dir / "bar1";
    bfs::ofstream file { filePath1 };
    file.close();

    const auto &filePath2 = m_dir / "bar2";
    file.open(filePath2);
    file.close();

    DirectoryWatcher watcher { m_dir };
    QSignalSpy addedSpy { &watcher, SIGNAL(fileAdded(const Path&)) };
    QSignalSpy deletedSpy { &watcher, SIGNAL(fileDeleted(const Path&)) };
    QSignalSpy modifiedSpy { &watcher, SIGNAL(fileModified(const Path&)) };

    QCOMPARE(addedSpy.size(), 0);
    QCOMPARE(deletedSpy.size(), 0);
    QCOMPARE(modifiedSpy.size(), 0);

    file.open(filePath1);
    file << "change change";
    file.close();
    QTest::qWait(100);

    file.open(filePath2);
    file << "this file also need to be changed.";
    file.close();
    QTest::qWait(100);

    QCOMPARE(addedSpy.size(), 0);
    QCOMPARE(deletedSpy.size(), 0);
    QCOMPARE(modifiedSpy.size(), 2);

    auto &&args = modifiedSpy.takeFirst();
    QCOMPARE(args.size(), 1);
    QCOMPARE(args[0].typeName(), "boost::filesystem::path");
    QCOMPARE(args[0].value<Path>(), filePath1);

    args = modifiedSpy.takeFirst();
    QCOMPARE(args.size(), 1);
    QCOMPARE(args[0].typeName(), "boost::filesystem::path");
    QCOMPARE(args[0].value<Path>(), filePath2);
}

void DirectoryWatcherTest::
testAddThenModify()
{
    namespace bfs = boost::filesystem;

    DirectoryWatcher watcher { m_dir };
    QSignalSpy addedSpy { &watcher, SIGNAL(fileAdded(const Path&)) };
    QSignalSpy deletedSpy { &watcher, SIGNAL(fileDeleted(const Path&)) };
    QSignalSpy modifiedSpy { &watcher, SIGNAL(fileModified(const Path&)) };

    QCOMPARE(addedSpy.size(), 0);
    QCOMPARE(deletedSpy.size(), 0);
    QCOMPARE(modifiedSpy.size(), 0);

    const auto &filePath = m_dir / "bar";
    bfs::ofstream file { filePath };
    file.close();
    QTest::qWait(100);

    file.open(filePath);
    file << "change contents";
    file.close();
    QTest::qWait(100);

    QCOMPARE(addedSpy.size(), 1);
    QCOMPARE(deletedSpy.size(), 0);
    QCOMPARE(modifiedSpy.size(), 1);

    auto &&args = addedSpy.takeFirst();
    QCOMPARE(args.size(), 1);
    QCOMPARE(args[0].typeName(), "boost::filesystem::path");
    QCOMPARE(args[0].value<Path>(), filePath);

    args = modifiedSpy.takeFirst();
    QCOMPARE(args.size(), 1);
    QCOMPARE(args[0].typeName(), "boost::filesystem::path");
    QCOMPARE(args[0].value<Path>(), filePath);
}

void DirectoryWatcherTest::
testModifyThenDelete()
{
    namespace bfs = boost::filesystem;

    const auto &filePath = m_dir / "bar";
    bfs::ofstream file { filePath };
    file.close();

    DirectoryWatcher watcher { m_dir };
    QSignalSpy addedSpy { &watcher, SIGNAL(fileAdded(const Path&)) };
    QSignalSpy deletedSpy { &watcher, SIGNAL(fileDeleted(const Path&)) };
    QSignalSpy modifiedSpy { &watcher, SIGNAL(fileModified(const Path&)) };

    QCOMPARE(addedSpy.size(), 0);
    QCOMPARE(deletedSpy.size(), 0);
    QCOMPARE(modifiedSpy.size(), 0);

    file.open(filePath);
    file << "modify contents";
    file.close();
    QTest::qWait(100);

    bfs::remove(filePath);
    QTest::qWait(100);

    QCOMPARE(addedSpy.size(), 0);
    QCOMPARE(deletedSpy.size(), 1);
    QCOMPARE(modifiedSpy.size(), 1);

    auto &&args = modifiedSpy.takeFirst();
    QCOMPARE(args.size(), 1);
    QCOMPARE(args[0].typeName(), "boost::filesystem::path");
    QCOMPARE(args[0].value<Path>(), filePath);

    args = deletedSpy.takeFirst();
    QCOMPARE(args.size(), 1);
    QCOMPARE(args[0].typeName(), "boost::filesystem::path");
    QCOMPARE(args[0].value<Path>(), filePath);
}

} // namespace stylist
