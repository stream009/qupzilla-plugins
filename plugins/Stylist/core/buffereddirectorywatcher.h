#ifndef BUFFERED_DIRECTORY_WATCHER_H
#define BUFFERED_DIRECTORY_WATCHER_H

#include "directorywatcher.h"
#include "signalbuffer.h"
#include "signalconverter.h"

#include "common/utility.h"

#include <boost/filesystem.hpp>

#include <QtCore/QObject>

namespace stylist {

class BufferedDirectoryWatcher : public QObject
{   Q_OBJECT
    using Path = boost::filesystem::path;

public:
    template<typename P>
    explicit BufferedDirectoryWatcher(P &&path)
        : m_watcher { std::forward<P>(path) }
    { init(); }

Q_SIGNALS:
    void fileAdded(const Path&) const;
    void fileDeleted(const Path&) const;
    void fileModified(const Path&) const;

private:
    void init();
    void establishPipeline(QObject &origin, const char* const signal,
                           const SignalConverter &,
                           const SignalBuffer &,
                           QObject &destination, const char* const method);
private:
    DirectoryWatcher m_watcher;
    SignalConverter m_addConverter;
    SignalConverter m_deleteConverter;
    SignalConverter m_modifyConverter;
    SignalBuffer m_addBuffer;
    SignalBuffer m_deleteBuffer;
    SignalBuffer m_modifyBuffer;
};

} // namespace stylist

#endif // BUFFERED_DIRECTORY_WATCHER_H
