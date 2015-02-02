include(../common.pri)
TEMPLATE = lib
CONFIG += staticlib

SOURCES = error.cpp \
          url.cpp

HEADERS = error.h \
          url.h \
          utility.h \
          stacktrace.h

# vim:ts=4 sw=4 sts=4 et:
