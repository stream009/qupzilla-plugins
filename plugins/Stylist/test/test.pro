include(../common.pri)
CONFIG += qtestlib debug
TEMPLATE = app
TARGET = 
DEPENDPATH += .
INCLUDEPATH += .
INCLUDEPATH += ..

PRECOMPILED_HEADER += stable.h

# Input
SOURCES += main.cpp

SOURCES += utilitytest.cpp \
           ../utility.cpp
HEADERS += utilitytest.h \
           ../utility.h

SOURCES += styletest.cpp \
           ../styles.cpp \
	   ../css/stylesheet.cpp \
	   ../css/documentrule.cpp \
	   ../css/condition.cpp
HEADERS += styletest.h \
           ../styles.h \
	   ../css/stylesheet.h \
	   ../css/documentrule.h \
	   ../css/condition.h

LIBS += -lboost_filesystem \
        -lboost_iostreams \
	-lboost_regex \
	-lboost_system

