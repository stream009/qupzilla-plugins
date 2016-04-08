include($$project_root/common.pri)
TEMPLATE = app

CONFIG += debug
QT += testlib

SOURCES = main.cpp

LIBS += -lcommon
POST_TARGETDEPS += $$project_root/common/libcommon.a

LIBS += -lgui
POST_TARGETDEPS += $$project_root/gui/libgui.a

LIBS += -lgui -lcommon
LIBS += -lboost_iostreams -lboost_filesystem -lboost_system

INCLUDEPATH += ../..
