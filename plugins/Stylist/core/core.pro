include(../common.pri)
TEMPLATE = lib
CONFIG += debug staticlib

SOURCES = buffereddirectorywatcher.cpp \
          directorywatcher.cpp \
          directorywatcher_p.cpp \
          signalbuffer.cpp \
          signalconverter.cpp \
          styles.cpp

HEADERS = buffereddirectorywatcher.h \
          directorywatcher.h \
          directorywatcher_p.h \
          signalbuffer.h \
          signalconverter.h \
          styles.h

# vim:ts=4 sw=4 sts=4 et:
