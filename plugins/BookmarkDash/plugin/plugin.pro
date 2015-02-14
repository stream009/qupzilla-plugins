TARGET = $$qtLibraryTarget(BookmarkDash)

include($$project_root/common.pri)

SOURCES += plugin.cpp \
	   settings.cpp

HEADERS += plugin.h \
	   settings.h

QMAKE_LFLAGS += -Wl,--no-undefined

LIBS += -lcommon
POST_TARGETDEPS += $$project_root/common/libcommon.a

LIBS += -lboost_filesystem -lboost_system

PLUGIN_DIR = $$PWD
include($$(QUPZILLA_SRCDIR)/src/plugins.pri)
