#include "plugin.hpp"

#include "core/stack_trace.hpp"

#include <cassert>
#include <iostream>

#include <signal.h>

#include <QtCore/QString>
#include <QtCore/QMessageLogContext>
#include <QtCore/QDir>
#include <QtCore/QDateTime>
#include <QtCore/QFile>
#include <QtCore/QTextStream>

#include <datapaths.h>

namespace stack_trace_dash {

using signal_handler_t = void(*)(int);
signal_handler_t s_previousHandler = nullptr;

void signalHandlerDash(int const sig)
{
    if (sig != SIGSEGV) {
        return;
    }

    static bool sigSegvServed = false;
    if (sigSegvServed) {
        abort();
    }
    sigSegvServed = true;

    std::cout << "QupZilla: Crashed :( Saving backtrace in " << qPrintable(DataPaths::path(DataPaths::Config)) << "/crashlog ..." << std::endl;

    StackTrace backtrace { 100, 2 };

    QDir dir(DataPaths::path(DataPaths::Config));
    if (!dir.exists()) {
        std::cout << qPrintable(DataPaths::path(DataPaths::Config)) << " does not exist" << std::endl;
        abort();
    }

    if (!dir.cd("crashlog")) {
        if (!dir.mkdir("crashlog")) {
            std::cout << "Cannot create " << qPrintable(DataPaths::path(DataPaths::Config)) << "crashlog directory!" << std::endl;
            abort();
        }

        dir.cd("crashlog");
    }

    const QDateTime currentDateTime = QDateTime::currentDateTime();

    QFile file(dir.absoluteFilePath("Crash-" + currentDateTime.toString(Qt::ISODate) + ".txt"));
    if (!file.open(QFile::WriteOnly | QFile::Truncate)) {
        std::cout << "Cannot open file " << qPrintable(file.fileName()) << " for writing!" << std::endl;
        abort();
    }

    QTextStream stream(&file);
    stream << "Time: " << currentDateTime.toString() << endl;
    stream << "Qt version: " << qVersion() << " (compiled with " << QT_VERSION_STR << ")" << endl;
    stream << "QupZilla version: " << Qz::VERSION << endl;
    stream << "WebKit version: " << qWebKitVersion() << endl;
    stream << endl;
    stream << "============== BACKTRACE ==============" << endl;

    try {
        stream << backtrace << endl;
    }
    catch (...) {
        s_previousHandler(sig);
    }

    file.close();

    std::cout << "Backtrace successfully saved in " << qPrintable(dir.absoluteFilePath(file.fileName())) << std::endl;
}

PluginSpec Plugin::
pluginSpec()
{
    PluginSpec spec;
    spec.name = "Stack Trace Dash";
    spec.info = "Debug helper";
    spec.description = "Alternative stack trace generator";
    spec.version = "0.1";
    spec.author = QString::fromUtf8("stream9 <stream009@gmail.com>");
    spec.hasSettings = false;

    return spec;
}

void Plugin::
init(InitState /*state*/, QString const& /*settingsPath*/)
{
    s_previousHandler = signal(SIGSEGV, signalHandlerDash);
}

void Plugin::unload()
{
    if (s_previousHandler) {
        signal(SIGSEGV, s_previousHandler);
    }
}

bool Plugin::
testPlugin()
{
    // Require the version that the plugin was built with
    return (Qz::VERSION == QLatin1String(QUPZILLA_VERSION));
}

} // namespace stack_trace_dash
