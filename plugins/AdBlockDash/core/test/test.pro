include($$project_root/common.pri)
TEMPLATE = app
CONFIG += debug
QT += network testlib webkit webkitwidgets

SOURCES = gtest_qt_main.cpp
#SOURCES += downloader.cpp 
SOURCES += http_server.cpp
SOURCES += adblock_dash.cpp 
SOURCES += subscription.cpp 
SOURCES += updater.cpp 
SOURCES += settings.cpp 

POST_TARGETDEPS += ../libcore.a $$project_root/common/libcommon.a

LIBS += -lcore -lcommon -lboost_iostreams -lboost_regex

LIBS += -L$$(QUPZILLA_SRCDIR)/bin -lQupZilla
LIBS += -L$$project_root/adblock/build -ladblock

LIBS += -lboost_filesystem -lboost_system
LIBS += -lgtest
