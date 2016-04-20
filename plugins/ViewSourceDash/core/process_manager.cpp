#include "process_manager.hpp"
#include "settings.hpp"

#include <iostream> //TODO remove

#include <boost/make_unique.hpp>
#include <boost/range/algorithm/remove_if.hpp>

#include <QtCore/QDebug>
#include <QtCore/QProcess>
#include <QtCore/QStringList>
#include <QtWidgets/QMessageBox>

namespace view_source_dash {

ProcessManager::
ProcessManager(const Settings &settings)
    : m_settings { settings }
{}

void ProcessManager::
launchViewer(const Path &path)
{
    QProcessPtr process { new QProcess };
    assert(process);

    QStringList args;
    args << path.c_str();

    using SignalT = void (QProcess::*)(const int, const QProcess::ExitStatus);
    this->connect(process.get(), static_cast<SignalT>(&QProcess::finished),
                  this,          &ProcessManager::onViewerFinished);
    this->connect(process.get(), &QProcess::errorOccurred,
                  this,          &ProcessManager::onProcessError);

    process->start(m_settings.viewer(), args);

    registerProcess(std::move(process));
}

void ProcessManager::
onViewerFinished(const int /*exitCode*/, const QProcess::ExitStatus /*status*/)
{
    auto* const process = dynamic_cast<QProcess*>(this->sender());
    assert(process);

    unregisterProcess(*process);
}

void ProcessManager::
onProcessError(const QProcess::ProcessError error)
{
    const auto &msg = QString { "Can't launch external editor [%1]: %2" }
        .arg(m_settings.viewer()).arg(error);

    QMessageBox::critical(nullptr, "Error", msg);

    auto* const process = dynamic_cast<QProcess*>(this->sender());
    assert(process);

    unregisterProcess(*process);
}

void ProcessManager::
registerProcess(QProcessPtr &&process)
{
    try {
        m_processes.push_back(std::move(process));
    }
    catch (...) {
        qDebug() << "ViewSourceDash: Warning: fail to register viewer process";
    }
}

void ProcessManager::
unregisterProcess(QProcess &process)
{
    const auto it = boost::remove_if(m_processes,
        [&process](const QProcessPtr &ptr) {
            return ptr.get() == &process;
        }
    );
    assert(it != m_processes.end()); //TODO better

    m_processes.erase(it);
}

} // namespace view_source_dash
