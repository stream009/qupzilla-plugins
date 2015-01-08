TARGET = $$qtLibraryTarget(LessChrome)
# OS/2 allows only 8 chars in TARGET
os2: TARGET = LessChrm

SOURCES += \
           lc_menubar.cpp \
	   lc_settings.cpp \
           plugin.cpp \
	   settingdialog.cpp \
	   tabwatcher.cpp \
	   toolbar.cpp \
           windowhandler.cpp

HEADERS += \
	   lc_menubar.h \
	   lc_settings.h \
	   plugin.h \
	   settingdialog.h \
	   tabwatcher.h \
	   toolbar.h \
           windowhandler.h

FORMS += settingdialog.ui

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
