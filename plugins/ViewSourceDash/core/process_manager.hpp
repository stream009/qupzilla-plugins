#ifndef VIEW_SOURCE_PROCESS_MANAGER_HPP
#define VIEW_SOURCE_PROCESS_MANAGER_HPP

#include <memory>
#include <vector>

#include <boost/filesystem/path.hpp>

#include <QtCore/QObject>
#include <QtCore/QProcess>

namespace view_source_dash {

class Settings;

class ProcessManager : public QObject
{
    Q_OBJECT

    struct Deleter {
        void operator()(void *ptr) {
            static_cast<QObject*>(ptr)->deleteLater();
        }
    };
    using QProcessPtr = std::unique_ptr<QProcess, Deleter>;

public:
    using Path = boost::filesystem::path;

public:
    ProcessManager(const Settings&);

    void launchViewer(const Path&);

private:
    Q_SLOT void onViewerFinished(
                    const int exitCode, const QProcess::ExitStatus);
    Q_SLOT void onProcessError(const QProcess::ProcessError);

    void registerProcess(QProcessPtr&&);
    void unregisterProcess(QProcess&);

private:
    const Settings &m_settings;
    std::vector<QProcessPtr> m_processes;
};

} // namespace view_source_dash

#endif // VIEW_SOURCE_PROCESS_MANAGER_HPP
