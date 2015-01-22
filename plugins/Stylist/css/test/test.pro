include(../../common.pri)
CONFIG += qtestlib debug 
TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .
INCLUDEPATH += ..

PRECOMPILED_HEADER += stable.h

# Input
SOURCES += ../../utility.cpp
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

LIBS += -lboost_filesystem \
        -lboost_iostreams \
        -lboost_regex \
	-lboost_system

