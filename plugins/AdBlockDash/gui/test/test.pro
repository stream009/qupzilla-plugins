include($$project_root/common.pri)
TEMPLATE = app

CONFIG += debug
QT += testlib

#SOURCES = setting_dialog.cpp
SOURCES = filter_list_model.cpp
SOURCES += gtest_qt_main.cpp

LIBS += -lgui
POST_TARGETDEPS += $$project_root/gui/libgui.a

LIBS += -lcore
POST_TARGETDEPS += $$project_root/core/libcore.a

LIBS += -lcommon
POST_TARGETDEPS += $$project_root/common/libcommon.a

LIBS += -lboost_iostreams -lboost_filesystem -lboost_regex -lboost_system
LIBS += -lgtest
