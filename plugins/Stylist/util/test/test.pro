include(../../common.pri)
TEMPLATE = app
CONFIG += qtestlib debug

SOURCES += main.cpp

SOURCES += signalbuffertest.cpp
HEADERS += signalbuffertest.h

SOURCES += directorywatchertest.cpp
HEADERS += directorywatchertest.h

LIBS += -L.. -lutil
POST_TARGETDEPS += ../libutil.a

LIBS += -lboost_filesystem \
        -lboost_system

# vim:ts=4 sw=4 sts=4 et:
