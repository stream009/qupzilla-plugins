include(../common.pri)
TEMPLATE = lib
CONFIG += staticlib

SOURCES = error.cpp

HEADERS = error.h \
          utility.h \
          stacktrace.h

# vim:ts=4 sw=4 sts=4 et:
