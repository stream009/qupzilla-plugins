TARGET = $$qtLibraryTarget(Stylist)
# OS/2 allows only 8 chars in TARGET
os2: TARGET = Stylist

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -Wno-deprecated-register

SOURCES += \
           error.cpp \
           plugin.cpp \
	   settings.cpp

HEADERS += \
	   error.h \
	   plugin.h \
	   settings.h \
	   stacktrace.h

PLUGIN_DIR = $$PWD
srcdir = $$(QUPZILLA_SRCDIR)
equals(srcdir, "") {
    include(../../plugins.pri)
}
else {
    include($$srcdir/src/plugins.pri)
}
