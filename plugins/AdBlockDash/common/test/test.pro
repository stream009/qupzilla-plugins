include($$project_root/common.pri)
TEMPLATE = app
CONFIG += debug

SOURCES += setting.cpp

LIBS += -lcommon
POST_TARGETDEPS += $$project_root/common/libcommon.a

LIBS += -lboost_filesystem -lboost_system
LIBS += -lgtest -lgtest_main

# vim:ts=4 sw=4 sts=4 et:
