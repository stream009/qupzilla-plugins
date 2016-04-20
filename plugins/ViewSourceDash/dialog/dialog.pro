include($$project_root/common.pri)

TARGET = $$qtLibraryTarget(ViewSourceDashSetting)

TEMPLATE = lib
CONFIG += plugin
QT += widgets

#FORMS = 
SOURCES = setting_dialog.cpp setting_dialog_plugin.cpp
HEADERS = setting_dialog.hpp setting_dialog_plugin.hpp setting_dialog_interface.hpp

LIBS += -L$$project_root/core -lcore
PRE_TARGETDEPS += $$project_root/core/libcore.a

PLUGIN_DIR = $$PWD
srcdir = $$(QUPZILLA_SRCDIR)
equals(srcdir, "") {
    include(../../plugins.pri)
}
else {
    include($$srcdir/src/plugins.pri)
}
