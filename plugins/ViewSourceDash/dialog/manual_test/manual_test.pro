include($$project_root/common.pri)
include($$project_root/qupzilla_includes.pri)

TEMPLATE = app
QT += widgets

#FORMS = 
SOURCES = main.cpp
#HEADERS = 

LIBS += -L$$project_root/core -lcore
PRE_TARGETDEPS += $$project_root/core/libcore.a

LIBS += -L$$(QUPZILLA_SRCDIR)/bin -lQupZilla
