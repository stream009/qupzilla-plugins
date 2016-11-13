TARGET = $$qtLibraryTarget(AdBlockDash)

include($$project_root/common.pri)
include($$project_root/qupzilla_includes.pri)

SOURCES += plugin.cpp \
           blocked_network_reply.cpp \
           scheme_handler.cpp \

HEADERS += plugin.hpp \
           blocked_network_reply.hpp \
           scheme_handler.hpp \

QMAKE_LFLAGS += -Wl,--no-undefined -Wl,--exclude-libs=ALL

LIBS += -L$$project_root/gui -lgui
POST_TARGETDEPS += $$project_root/gui/libgui.a

LIBS += -lcore
POST_TARGETDEPS += $$project_root/core/libcore.a

LIBS += -lcommon
POST_TARGETDEPS += $$project_root/common/libcommon.a

LIBS += -ladblock -lstream9_qt

LIBS += -Wl,-Bstatic \
    -lboost_filesystem \
    -lboost_iostreams \
    -lboost_regex \
    -lboost_system \
    -Wl,-Bdynamic \

PLUGIN_DIR = $$PWD
include($$(QUPZILLA_SRCDIR)/src/plugins.pri)
