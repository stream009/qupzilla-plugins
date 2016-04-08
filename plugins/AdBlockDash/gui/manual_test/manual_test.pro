include($$project_root/common.pri)
TEMPLATE = app

CONFIG += debug
QT += testlib network webkit

INCLUDEPATH += $$project_root/lib
DEPENDPATH += $$project_root/lib

TARGET = setting_dialog
SOURCES = setting_dialog.cpp

LIBS += -lgui
POST_TARGETDEPS += $$project_root/gui/libgui.a

LIBS += -lcore
POST_TARGETDEPS += $$project_root/core/libcore.a

LIBS += -lcommon
POST_TARGETDEPS += $$project_root/common/libcommon.a

LIBS += -lboost_iostreams -lboost_filesystem -lboost_regex -lboost_system

LIBS += \
    -L$$project_root/lib/qt_widget_list/build -lwidget_list \
