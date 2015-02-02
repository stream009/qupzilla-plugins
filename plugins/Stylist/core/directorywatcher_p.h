#ifndef STYLIST_DIRECTORY_WATCHER_P_H
#define STYLIST_DIRECTORY_WATCHER_P_H

#include "directorywatcher.h"

#include <vector>

#include <boost/filesystem.hpp>

#include <QtCore/QObject>
#include <QtCore/QFileSystemWatcher>

namespace stylist {

class DirectoryWatcher::Impl : public QObject
{
    Q_OBJECT
public:
    using Path = DirectoryWatcher::Path;
    using DirectoryEntry = boost::filesystem::directory_entry;

    Impl(DirectoryWatcher &p);

private:
    Path &path() { return m_p.m_path; }

private Q_SLOTS:
    void slotDirectoryChanged(const QString &);
    void slotFileChanged(const QString &);

private:
    DirectoryWatcher &m_p;

    QFileSystemWatcher m_fsWatcher;
    std::vector<DirectoryEntry> m_entries;
};

} // namespace stylist

#endif // STYLIST_DIRECTORY_WATCHER_P_H
