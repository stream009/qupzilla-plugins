TARGET = $$qtLibraryTarget(Stylist)
# OS/2 allows only 8 chars in TARGET
os2: TARGET = Stylist

include(common.pri)

SOURCES += \
           error.cpp \
           plugin.cpp \
	   settings.cpp \
	   styles.cpp \
	   utility.cpp \
	   webframe.cpp \
	   webpage.cpp

HEADERS += \
	   error.h \
	   plugin.h \
	   settings.h \
	   stacktrace.h \
	   styles.h \
	   utility.h \
	   webframe.h \
	   webpage.h

PRECOMPILED_HEADER += stable.h

LIBS += -L./css -lcss
POST_TARGETDEPS += css/libcss.a

LIBS += -L./gui -lgui
POST_TARGETDEPS += gui/libgui.a

LIBS += \
           -lboost_filesystem \
           -lboost_iostreams \
           -lboost_regex \
           -lboost_system

PLUGIN_DIR = $$PWD
srcdir = $$(QUPZILLA_SRCDIR)
equals(srcdir, "") {
    include(../../plugins.pri)
}
else {
    include($$srcdir/src/plugins.pri)
}
