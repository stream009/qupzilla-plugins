TARGET = $$qtLibraryTarget(LessChrome)
# OS/2 allows only 8 chars in TARGET
os2: TARGET = LessChrm

SOURCES += \
           error.cpp \
           lc_menubar.cpp \
	   lc_settings.cpp \
           plugin.cpp \
	   settingdialog.cpp \
	   tabwatcher.cpp \
	   toolbar.cpp \
           windowhandler.cpp

HEADERS += \
	   error.h \
	   lc_menubar.h \
	   lc_settings.h \
	   plugin.h \
	   settingdialog.h \
	   stacktrace.h \
	   tabwatcher.h \
	   toolbar.h \
           windowhandler.h

FORMS += settingdialog.ui

PLUGIN_DIR = $$PWD
srcdir = $$(QUPZILLA_SRCDIR)
equals(srcdir, "") {
    include(../../plugins.pri)
}
else {
    include($$srcdir/src/plugins.pri)
}
