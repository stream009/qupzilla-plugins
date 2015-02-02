include(../common.pri)
TEMPLATE = lib
CONFIG += debug staticlib

INCLUDEPATH += ..
INCLUDEPATH += ../include

SOURCES = buffereddirectorywatcher.cpp \
          error.cpp \
          directorywatcher.cpp \
          directorywatcher_p.cpp \
          signalbuffer.cpp \
          signalconverter.cpp \
          styles.cpp \
          url.cpp

HEADERS = buffereddirectorywatcher.h \
          error.h \
          directorywatcher.h \
          directorywatcher_p.h \
          signalbuffer.h \
          signalconverter.h \
          stacktrace.h \
          styles.h \
          url.h \
          utility.h

# vim:ts=4 sw=4 sts=4 et:
