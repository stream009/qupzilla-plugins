#ifndef DIRECTORY_WATCHER_H
#define DIRECTORY_WATCHER_H

#include <boost/filesystem.hpp>

#include <QtCore/QObject>
#include <QtCore/QMetaType>

namespace stylist {

class DirectoryWatcher : public QObject
{
    Q_OBJECT
    using Path = boost::filesystem::path;

public:
    explicit DirectoryWatcher(const Path &path);
    explicit DirectoryWatcher(Path &&path);

    virtual ~DirectoryWatcher();

Q_SIGNALS:
    void fileAdded(const Path &);
    void fileDeleted(const Path &);
    void fileModified(const Path &);

private:
    Path m_path;

    class Impl;
    std::unique_ptr<Impl> m_p;
};

} // namespace stylist

Q_DECLARE_METATYPE(boost::filesystem::path);

#endif // DIRECTORY_WATCHER_H
