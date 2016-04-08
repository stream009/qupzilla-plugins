include($$project_root/common.pri)
include($$project_root/qupzilla_includes.pri)

TEMPLATE = lib
CONFIG += staticlib
QT += network webkit webkitwidgets

QMAKE_CXXFLAGS += -g

SOURCES += \
    adblock_dash.cpp \
    frame_adaptor.cpp \
    downloader.cpp \
    settings.cpp \
    filter_set.cpp \
    subscription.cpp \
    text_file.cpp \
    updater.cpp \

HEADERS += \
    adblock_dash.hpp \
    frame_adaptor.hpp \
    downloader.hpp \
    settings.hpp \
    filter_set.hpp \
    subscription.hpp \
    text_file.hpp \
    updater.hpp \
