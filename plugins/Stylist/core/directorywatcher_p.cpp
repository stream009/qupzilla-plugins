#include "directorywatcher_p.h"

#include <algorithm>

#include <boost/function_output_iterator.hpp>
#include <boost/iterator/filter_iterator.hpp>

#include <QtCore/QDebug>

namespace stylist {

namespace {

namespace bfs = boost::filesystem;

struct IsRegularFile {
    bool operator()(const bfs::directory_entry &e) const {
        return e.status().type() == bfs::regular_file;
    }
};

using RegularFileIterator =
    boost::filter_iterator<IsRegularFile, bfs::directory_iterator>;

static RegularFileIterator
makeRegularFileIterator(const bfs::path &path)
{
    return boost::make_filter_iterator(
        IsRegularFile {}, bfs::directory_iterator { path });
}

} // unnamed namespace

DirectoryWatcher::Impl::
Impl(DirectoryWatcher &p)
    : m_p { p }
{
    //qDebug() << __func__ << path().c_str();
    namespace bfs = boost::filesystem;

    m_fsWatcher.addPath(path().c_str());
    for (RegularFileIterator it = makeRegularFileIterator(path()), end {};
            it != end; ++it)
    {
        m_fsWatcher.addPath(it->path().c_str());
        m_entries.push_back(std::move(*it));
    }
    std::sort(m_entries.begin(), m_entries.end());

    this->connect(&m_fsWatcher, SIGNAL(directoryChanged(const QString&)),
        this,                   SLOT(slotDirectoryChanged(const QString&)));
    this->connect(&m_fsWatcher, SIGNAL(fileChanged(const QString&)),
                  this,         SLOT(slotFileChanged(const QString&)));
}

void DirectoryWatcher::Impl::
slotDirectoryChanged(const QString &pathStr)
{
    //qDebug() << __func__ << pathStr;
    namespace bfs = boost::filesystem;

    const Path path { pathStr.toUtf8().constData() };

    std::vector<DirectoryEntry> newEntries {
        makeRegularFileIterator(this->path()), {}
    };
    std::sort(newEntries.begin(), newEntries.end());

    std::set_difference(
        newEntries.begin(), newEntries.end(),
        m_entries.begin(), m_entries.end(),
        boost::make_function_output_iterator(
            // Unfortunately, function_outpu_iterator doesn't support rvalue.
            // But, at the version 1.57 of boost, directory_entry doesn't
            // support move semantics either anyway.
            [this] (const DirectoryEntry &e) {
                const auto &path = e.path();
                m_fsWatcher.addPath(path.c_str());
                Q_EMIT m_p.fileAdded(path);
            }
        )
    );

    std::set_difference(
        m_entries.begin(), m_entries.end(),
        newEntries.begin(), newEntries.end(),
        boost::make_function_output_iterator(
            [this] (const DirectoryEntry &e) {
                Q_EMIT m_p.fileDeleted(e.path());
            }
        )
    );

    std::swap(m_entries, newEntries);
}

void DirectoryWatcher::Impl::
slotFileChanged(const QString &pathStr)
{
    //qDebug() << __func__ << pathStr;
    namespace bfs = boost::filesystem;

    const Path path { pathStr.toUtf8().constData() };

    if (bfs::exists(path)) {
        Q_EMIT m_p.fileModified(path);
    }
    // File deletion event will be handled in slotDirectoryChanged.
    // Rename event will be handled there too.
}

} // namespace stylist
