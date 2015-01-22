TARGET = $$qtLibraryTarget(Stylist)
# OS/2 allows only 8 chars in TARGET
os2: TARGET = Stylist

CONFIG += precompile_header

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -Wno-deprecated-register

SOURCES += \
           error.cpp \
           plugin.cpp \
	   settings.cpp \
	   styles.cpp \
	   utility.cpp \
	   webframe.cpp

HEADERS += \
	   error.h \
	   plugin.h \
	   settings.h \
	   stacktrace.h \
	   styles.h \
	   utility.h \
	   webframe.h \
	   webpage.h

SOURCES += \
           css/condition.cpp \
	   css/documentrule.cpp \
	   css/stylesheet.cpp

HEADERS += \
           css/condition.h \
	   css/documentrule.h \
	   css/stylesheet.h

LIBS += \
           -lboost_filesystem \
           -lboost_iostreams \
           -lboost_regex \
           -lboost_system

PRECOMPILED_DIR = build
PRECOMPILED_HEADER += stable.h

PLUGIN_DIR = $$PWD
srcdir = $$(QUPZILLA_SRCDIR)
equals(srcdir, "") {
    include(../../plugins.pri)
}
else {
    include($$srcdir/src/plugins.pri)
}
