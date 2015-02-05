include(../common.pri)
TEMPLATE = lib
CONFIG += staticlib

SOURCES = settingdialog.cpp \
          stylesitemmodel.cpp \
          stylesview.cpp

HEADERS = settingdialog.h \
          stylesitemmodel.h \
          stylesview.h

FORMS = settingdialog.ui

# vim:ts=4 sw=4 sts=4 et:
