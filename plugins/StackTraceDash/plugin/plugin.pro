include($$(QUPZILLA_SRCDIR)/src/plugins.pri)
include($$project_root/common.pri)

TARGET = StackTraceDash

QT += network 

SOURCES += plugin.cpp
HEADERS += plugin.hpp

LIBS += -L$$project_root/core -lcore
POST_TARGETDEPS += $$project_root/core/libcore.a

LIBS += -ldl
