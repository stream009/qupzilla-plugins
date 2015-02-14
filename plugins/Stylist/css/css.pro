include(../common.pri)
TEMPLATE = lib
CONFIG += staticlib

SOURCES = condition.cpp \
          documentrule.cpp \
          stylesheet.cpp
HEADERS = condition.h \
          documentrule.h \
          stylesheet.h

# vim:ts=4 sw=4 sts=4 et:
