include(../common.pri)
CONFIG += qtestlib debug
TEMPLATE = app

INCLUDEPATH += ..

include($$(QUPZILLA_SRCDIR)/src/plugins.pri)

# Input
SOURCES += main.cpp

SOURCES += styletest.cpp \
           ../styles.cpp \
	   ../plugin.cpp
HEADERS += styletest.h \
           ../styles.h \
	   ../plubin.h

LIBS += -L../css -lcss
POST_TARGETDEPS += ../css/libcss.a

LIBS += -L../util -lutil
POST_TARGETDEPS += ../util/libutil.a

LIBS += -lboost_filesystem \
        -lboost_iostreams \
	-lboost_regex \
	-lboost_system
