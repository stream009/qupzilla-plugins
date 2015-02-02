include(../../common.pri)
CONFIG += qtestlib debug 
TEMPLATE = app

INCLUDEPATH += ../..

# Input
HEADERS += ../../utility.h

SOURCES += main.cpp

SOURCES += conditiontest.cpp
HEADERS += conditiontest.h

SOURCES += documentruletest.cpp
HEADERS += documentruletest.h

SOURCES += stylesheettest.cpp
HEADERS += stylesheettest.h 

LIBS += -L.. -lcss
POST_TARGETDEPS += ../libcss.a

LIBS += -L../../core -lcore
POST_TARGETDEPS += ../../core/libcore.a

LIBS += -lboost_filesystem \
        -lboost_iostreams \
        -lboost_regex \
	-lboost_system

