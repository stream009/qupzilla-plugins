#ifndef STYLIST_DIRECTORY_WATCHER_TEST_H
#define STYLIST_DIRECTORY_WATCHER_TEST_H

#include <boost/filesystem.hpp>

#include <QtCore/QObject>

namespace stylist {

class DirectoryWatcherTest : public QObject
{
    Q_OBJECT
    using Path = boost::filesystem::path;

private Q_SLOTS:
    void init();
    void cleanup();

    void testConstructor();
    void testFileAdded();
    void testMultipleFilesAdded();
    void testFileDeleted();
    void testMultipleFilesDeleted();
    void testFileRenamed();
    void testFileModified();
    void testMultipleFilesModified();

    void testAddThenModify();
    void testModifyThenDelete();

private:
    static const Path m_dir;
};

} // namespace stylist

#endif // STYLIST_DIRECTORY_WATCHER_TEST_H
