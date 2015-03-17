include($$project_root/common.pri)
include($$project_root/qupzilla_includes.pri)
TEMPLATE = lib
CONFIG += debug staticlib
QT += network webkit

SOURCES += menu.cpp bar.cpp context_menu.cpp bookmark_dialog.cpp \
           folder_dialog.cpp rootmenu.cpp menubutton.cpp
HEADERS += menu.h bar.h context_menu.h bookmark_dialog.h \
           folder_dialog.h view.h rootmenu.h menubutton.h
FORMS += bookmark_dialog.ui folder_dialog.ui

LIBS += -lcommon
POST_TARGETDEPS += $$project_root/common/libcommon.a

DEPENDPATH += $$INCLUDEPATH \
              $$(QUPZILLA_SRCDIR)/src/lib/data \

# vim:ts=4 sw=4 sts=4 et:
