include(../../common.pri)
TEMPLATE = app
CONFIG += qtestlib debug

SOURCES += main.cpp

SOURCES += signalbuffertest.cpp
HEADERS += signalbuffertest.h \
           ../signalbuffer.h

SOURCES += directorywatchertest.cpp
HEADERS += directorywatchertest.h \
           ../directorywatcher.h

SOURCES += buffereddirectorywatchertest.cpp
HEADERS += buffereddirectorywatchertest.h \
           ../buffereddirectorywatcher.h

INCLUDEPATH += ../..

LIBS += -L.. -lcore
POST_TARGETDEPS += ../libcore.a

LIBS += -lboost_filesystem \
        -lboost_system

# vim:ts=4 sw=4 sts=4 et:
