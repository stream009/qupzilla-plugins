TARGET = $$qtLibraryTarget(Stylist)
# OS/2 allows only 8 chars in TARGET
os2: TARGET = Stylist

include(../common.pri)

SOURCES += \
           plugin.cpp \
	   settings.cpp \
	   webframe.cpp \
	   webpage.cpp

HEADERS += \
	   plugin.h \
	   settings.h \
	   webframe.h \
	   webpage.h

LIBS += -L../core -lcore
POST_TARGETDEPS += ../core/libcore.a

LIBS += -L../css -lcss
POST_TARGETDEPS += ../css/libcss.a

LIBS += -L../gui -lgui
POST_TARGETDEPS += ../gui/libgui.a

LIBS +=  -lboost_filesystem \
         -lboost_iostreams \
         -lboost_regex \
	 -lboost_serialization \
         -lboost_system

subdirs.commands = (cd css; make) && (cd util; make) && (cd gui; make)
QMAKE_EXTRA_TARGETS += subdirs

PLUGIN_DIR = $$PWD
include($$(QUPZILLA_SRCDIR)/src/plugins.pri)
