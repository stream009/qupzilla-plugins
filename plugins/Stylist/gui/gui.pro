include(../common.pri)
TARGET = gui
TEMPLATE = lib
CONFIG += staticlib

SOURCES = settingdialog.cpp \
          stylesitemmodel.cpp

HEADERS = settingdialog.h \
          stylesitemmodel.h

FORMS = settingdialog.ui

INCLUDEPATH += ..
INCLUDEPATH += $$(QUPZILLA_SRCDIR)/src/lib/webkit \
               $$(QUPZILLA_SRCDIR)/src/lib/app \
               $$(QUPZILLA_SRCDIR)/src/lib/autofill \
               $$(QUPZILLA_SRCDIR)/src/lib/plugins \
               /usr/include/qt4/QtWebKit \
               /usr/include/qt4/QtNetwork

# vim:ts=4 sw=4 sts=4 et:
