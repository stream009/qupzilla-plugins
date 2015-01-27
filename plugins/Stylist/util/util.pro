include(../common.pri)
TEMPLATE = lib
CONFIG += debug staticlib

SOURCES = directorywatcher.cpp \
          directorywatcher_p.cpp \
          signalbuffer.cpp
HEADERS = directorywatcher.h \
          directorywatcher_p.h \
          signalbuffer.h

# vim:ts=4 sw=4 sts=4 et:
