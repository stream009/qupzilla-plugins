include($$project_root/common.pri)
TEMPLATE = app
CONFIG += qtestlib debug

SOURCES += main.cpp

SOURCES += signalbuffertest.cpp
HEADERS += signalbuffertest.h

SOURCES += directorywatchertest.cpp
HEADERS += directorywatchertest.h

SOURCES += buffereddirectorywatchertest.cpp
HEADERS += buffereddirectorywatchertest.h

SOURCES += styletest.cpp
HEADERS += styletest.h

POST_TARGETDEPS += ../libcore.a \
                   $$project_root/common/libcommon.a \
                   $$project_root/css/libcss.a

LIBS += -lcore -lcss -lcommon
LIBS += -lboost_filesystem \
        -lboost_iostreams \
        -lboost_regex \
        -lboost_serialization \
        -lboost_system

# vim:ts=4 sw=4 sts=4 et:
