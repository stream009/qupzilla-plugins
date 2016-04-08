include(../common.pri)
TEMPLATE = lib
CONFIG += staticlib

QT += widgets

SOURCES = settingactions.cpp \
          settingdialog.cpp \
          stylesitemmodel.cpp \
          stylesview.cpp

HEADERS = settingactions.h \
          settingdialog.h \
          stylesitemmodel.h \
          stylesview.h

SOURCES += action/addstyle.cpp \
           action/enablestyle.cpp \
           action/openstyle.cpp \
           action/removestyle.cpp \
           action/renamestyle.cpp

HEADERS += action/addstyle.h \
           action/enablestyle.h \
           action/openstyle.h \
           action/removestyle.h \
           action/renamestyle.h

FORMS = settingdialog.ui

# vim:ts=4 sw=4 sts=4 et:
