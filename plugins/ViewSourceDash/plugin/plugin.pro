include($$project_root/common.pri)

TARGET = ViewSourceDash

SOURCES += plugin.cpp
HEADERS += plugin.hpp

LIBS += -L$$project_root/core -lcore
PRE_TARGETDEPS += $$project_root/core/libcore.a

LIBS += -lboost_filesystem -lboost_system

PLUGIN_DIR = $$PWD
srcdir = $$(QUPZILLA_SRCDIR)
equals(srcdir, "") {
    include(../../plugins.pri)
}
else {
    include($$srcdir/src/plugins.pri)
}
