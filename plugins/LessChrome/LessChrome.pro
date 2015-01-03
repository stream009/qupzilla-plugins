TARGET = $$qtLibraryTarget(LessChrome)
# OS/2 allows only 8 chars in TARGET
os2: TARGET = LessChrome

SOURCES += plugin.cpp \
           windowhandler.cpp \
	   toolbar.cpp
HEADERS += plugin.h \
           windowhandler.h \
	   toolbar.h
#RESOURCES += lesschrome.qrc

#TRANSLATIONS += \
#    translations/ar_SA.ts \

PLUGIN_DIR = $$PWD
srcdir = $$(QUPZILLA_SRCDIR)
equals(srcdir, "") {
    include(../../plugins.pri)
}
else {
    include($$srcdir/src/plugins.pri)
}
