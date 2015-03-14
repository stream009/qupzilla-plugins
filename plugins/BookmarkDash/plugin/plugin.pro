TARGET = $$qtLibraryTarget(BookmarkDash)

include($$project_root/common.pri)

SOURCES += plugin.cpp \
	       settings.cpp \
           settingsmenu.cpp \
	       windowadaptor.cpp \
           menuadaptor.cpp \
           toolbaradaptor.cpp \

HEADERS += plugin.h \
	       settings.h \
           settingsmenu.h \
	       windowadaptor.h \
           menuadaptor.h \
           toolbaradaptor.h \

QMAKE_LFLAGS += -Wl,--no-undefined

LIBS += -lcommon
POST_TARGETDEPS += $$project_root/common/libcommon.a

LIBS += -lbookmark
POST_TARGETDEPS += $$project_root/bookmark/libbookmark.a

LIBS += -lmixin
POST_TARGETDEPS += $$project_root/qt_mixin/lib/libmixin.a

LIBS += -lboost_filesystem -lboost_system

PLUGIN_DIR = $$PWD
include($$(QUPZILLA_SRCDIR)/src/plugins.pri)

# vim:ts=4 sw=4 sts=4 et:
