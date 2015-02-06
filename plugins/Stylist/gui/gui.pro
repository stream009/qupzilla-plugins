include(../common.pri)
TEMPLATE = lib
CONFIG += staticlib

SOURCES = settingdialog.cpp \
          stylesitemmodel.cpp \
          stylesview.cpp

SOURCES += action/addstyle.cpp \
           action/openstyle.cpp

HEADERS = settingdialog.h \
          stylesitemmodel.h \
          stylesview.h

HEADERS += action/addstyle.h \
           action/openstyle.h

FORMS = settingdialog.ui

# vim:ts=4 sw=4 sts=4 et:
