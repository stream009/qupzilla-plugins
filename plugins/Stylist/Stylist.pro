TARGET = $$qtLibraryTarget(Stylist)
# OS/2 allows only 8 chars in TARGET
os2: TARGET = Stylist

include(common.pri)

INCLUDEPATH += include

SOURCES += \
           plugin.cpp \
	   settings.cpp \
	   styles.cpp \
	   webframe.cpp \
	   webpage.cpp

HEADERS += \
	   plugin.h \
	   settings.h \
	   styles.h \
	   utility.h \
	   webframe.h \
	   webpage.h

LIBS += -L./css -lcss
POST_TARGETDEPS += css/libcss.a

LIBS += -L./gui -lgui
POST_TARGETDEPS += gui/libgui.a

LIBS += -L./util -lutil
POST_TARGETDEPS += util/libutil.a

LIBS += \
           -lboost_filesystem \
           -lboost_iostreams \
           -lboost_regex \
	   -lboost_serialization \
           -lboost_system

subdirs.commands = (cd css; make) && (cd util; make) && (cd gui; make)
QMAKE_EXTRA_TARGETS += subdirs

PLUGIN_DIR = $$PWD
srcdir = $$(QUPZILLA_SRCDIR)
equals(srcdir, "") {
    include(../../plugins.pri)
}
else {
    include($$srcdir/src/plugins.pri)
}
