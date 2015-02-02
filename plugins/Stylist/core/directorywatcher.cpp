#include "directorywatcher.h"

#include "directorywatcher_p.h"

namespace stylist {

DirectoryWatcher::
DirectoryWatcher(const Path &path)
    : m_path { path },
      m_p { new Impl { *this } }
{
    //TODO Path has to exist whether relative or absolute
    qRegisterMetaType<Path>("Path");
    qRegisterMetaType<Path>("boost::filesystem::path");
}

DirectoryWatcher::
DirectoryWatcher(Path &&path)
    : m_path { std::move(path) },
      m_p { new Impl { *this } }
{
    qRegisterMetaType<Path>("Path");
    qRegisterMetaType<Path>("boost::filesystem::path");
}

DirectoryWatcher::~DirectoryWatcher() = default;

} // namespace stylist
