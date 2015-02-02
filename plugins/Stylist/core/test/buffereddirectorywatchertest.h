#ifndef STYLIST_BUFFERED_DIRECTORY_WATCHER_TEST_H
#define STYLIST_BUFFERED_DIRECTORY_WATCHER_TEST_H

#include <boost/filesystem.hpp>

#include <QtCore/QObject>

namespace stylist {

class BufferedDirectoryWatcherTest : public QObject
{
    Q_OBJECT
    using Path = boost::filesystem::path;

private Q_SLOTS:
    void init();
    void cleanup();

    void testConstructor();
    void testFileAdded();
    void testFileDeleted();
    void testFileModified();

private:
    static const Path m_dir;
};

} // namespace stylist

#endif // STYLIST_BUFFERED_DIRECTORY_WATCHER_TEST_H
