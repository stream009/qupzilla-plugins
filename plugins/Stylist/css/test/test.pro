include(../../common.pri)
CONFIG += qtestlib debug 
TEMPLATE = app
QT += widgets

# Input
SOURCES += main.cpp

SOURCES += conditiontest.cpp
HEADERS += conditiontest.h

SOURCES += documentruletest.cpp
HEADERS += documentruletest.h

SOURCES += stylesheettest.cpp
HEADERS += stylesheettest.h 

POST_TARGETDEPS += ../libcss.a \
                   $$project_root/common/libcommon.a

LIBS += -lcss -lcommon

LIBS += -lboost_filesystem \
        -lboost_iostreams \
        -lboost_regex \
	-lboost_system

