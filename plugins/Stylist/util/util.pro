include(../common.pri)
TEMPLATE = lib
CONFIG += debug staticlib

SOURCES = error.cpp \
          directorywatcher.cpp \
          directorywatcher_p.cpp \
          signalbuffer.cpp \
          url.cpp

HEADERS = error.h \
          directorywatcher.h \
          directorywatcher_p.h \
          signalbuffer.h \
          stacktrace.h \
          url.h

# vim:ts=4 sw=4 sts=4 et:
