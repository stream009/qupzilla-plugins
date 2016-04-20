include($$project_root/common.pri)
include($$project_root/qupzilla_includes.pri)

TEMPLATE = lib
CONFIG += staticlib
QT += widgets

#FORMS = 
SOURCES += settings.cpp process_manager.cpp view_source.cpp
HEADERS += settings.hpp process_manager.hpp view_source.hpp
