TARGET = $$qtLibraryTarget(LessChrome)
# OS/2 allows only 8 chars in TARGET
os2: TARGET = LessChrm

SOURCES += plugin.cpp \
           windowhandler.cpp \
	   toolbar.cpp \
	   tabwatcher.cpp \
	   lc_settings.cpp \
	   settingdialog.cpp

HEADERS += plugin.h \
           windowhandler.h \
	   toolbar.h \
	   tabwatcher.h \
	   lc_settings.h \
	   settingdialog.h

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
