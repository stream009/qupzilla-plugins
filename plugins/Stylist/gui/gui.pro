include(../common.pri)
TARGET = gui
TEMPLATE = lib
CONFIG += staticlib

SOURCES = settingdialog.cpp
HEADERS = settingdialog.h
FORMS = settingdialog.ui
PRECOMPILED_HEADER = stable.h
