include($$project_root/common.pri)
include($$project_root/qupzilla_includes.pri)
TEMPLATE = lib
CONFIG += staticlib
QT += network webkit

SOURCES = error.cpp tabwatcher.cpp

HEADERS = error.h \
          utility.h \
          stacktrace.h \
          tabwatcher.h

DEPENDPATH += $$INCLUDEPATH \
              $$(QUPZILLA_SRCDIR)/src/lib/data \

# vim:ts=4 sw=4 sts=4 et:
