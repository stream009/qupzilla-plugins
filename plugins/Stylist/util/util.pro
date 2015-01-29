include(../common.pri)
TEMPLATE = lib
CONFIG += debug staticlib

INCLUDEPATH += ..

SOURCES = buffereddirectorywatcher.cpp \
          error.cpp \
          directorywatcher.cpp \
          directorywatcher_p.cpp \
          signalbuffer.cpp \
          signalconverter.cpp \
          url.cpp

HEADERS = buffereddirectorywatcher.h \
          error.h \
          directorywatcher.h \
          directorywatcher_p.h \
          signalbuffer.h \
          signalconverter.h \
          stacktrace.h \
          url.h

# vim:ts=4 sw=4 sts=4 et:
