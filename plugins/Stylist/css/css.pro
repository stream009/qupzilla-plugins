include(../common.pri)
TEMPLATE = lib
CONFIG += staticlib

SOURCES = condition.cpp \
          documentrule.cpp \
          stylesheet.cpp
HEADERS = condition.h \
          documentrule.h \
          stylesheet.h
PRECOMPILED_HEADER = stable.h

# vim:ts=4 sw=4 sts=4 et:
