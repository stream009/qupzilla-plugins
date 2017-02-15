#include "plugin.hpp"

#include <cassert>

#include <syslog.h>

#include <QtCore/QString>
#include <QtCore/QMessageLogContext>

#include <iostream>

namespace syslog_dash {

static QtMessageHandler previousHandler = nullptr;

static void
handleMessage(QtMsgType const type, QMessageLogContext const& cxt,
                                    QString const& msg)
{
    QByteArray localMsg = msg.toLocal8Bit();
    localMsg.replace('%', "%%");

    switch (type) {
    case QtDebugMsg:
        syslog(LOG_DEBUG, localMsg.constData());
        break;
    case QtInfoMsg:
        syslog(LOG_INFO, localMsg.constData());
        break;
    case QtWarningMsg:
        syslog(LOG_WARNING, localMsg.constData());
        break;
    case QtCriticalMsg:
        syslog(LOG_CRIT, localMsg.constData());
        break;
    case QtFatalMsg:
        syslog(LOG_EMERG, localMsg.constData());
        abort();
        break;
    default:
        assert(false && "SyslogDash: Unknown QtMsgType");
    }

    assert(previousHandler);
    previousHandler(type, cxt, msg);
}

PluginSpec Plugin::
pluginSpec()
{
    PluginSpec spec;
    spec.name = "Syslog Dash";
    spec.info = "Debug helper";
    spec.description = "Redirect debug message to syslog facility.";
    spec.version = "0.1";
    spec.author = QString::fromUtf8("stream9 <stream009@gmail.com>");
    spec.hasSettings = false;

    return spec;
}

void Plugin::
init(InitState /*state*/, QString const& /*settingsPath*/)
{
    previousHandler = qInstallMessageHandler(handleMessage);
}

void Plugin::unload()
{
    qInstallMessageHandler(previousHandler);
}

bool Plugin::
testPlugin()
{
    // Require the version that the plugin was built with
    return (Qz::VERSION == QLatin1String(QUPZILLA_VERSION));
}

} // namespace syslog_dash
