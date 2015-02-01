include(../common.pri)
CONFIG += qtestlib debug
TEMPLATE = app

QT += network webkit

INCLUDEPATH += ..
INCLUDEPATH += ../include

# Input
SOURCES += main.cpp

SOURCES += styletest.cpp 
HEADERS += styletest.h

#SOURCES += ../styles.cpp \
#           ../plugin.cpp

LIBS += ../build/styles.o \
        ../build/moc_styles.o

LIBS += -L../css -lcss
POST_TARGETDEPS += ../css/libcss.a

LIBS += -L../util -lutil
POST_TARGETDEPS += ../util/libutil.a

LIBS += -lboost_filesystem \
        -lboost_iostreams \
	-lboost_regex \
	-lboost_serialization \
	-lboost_system
