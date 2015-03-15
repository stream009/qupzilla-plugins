include($$project_root/common.pri)
TEMPLATE = lib
CONFIG += debug staticlib

SOURCES += menu.cpp bar.cpp context_menu.cpp bookmark_dialog.cpp \
           folder_dialog.cpp
HEADERS += menu.h bar.cpp context_menu.h bookmark_dialog.h \
           folder_dialog.h view.h
FORMS += bookmark_dialog.ui folder_dialog.ui

LIBS += -lcommon
POST_TARGETDEPS += $$project_root/common/libcommon.a

INCLUDEPATH += $$(QUPZILLA_SRCDIR)/src/lib/3rdparty \
               $$(QUPZILLA_SRCDIR)/src/lib/adblock \
               $$(QUPZILLA_SRCDIR)/src/lib/app \
               $$(QUPZILLA_SRCDIR)/src/lib/autofill \
               $$(QUPZILLA_SRCDIR)/src/lib/bookmarks \
               $$(QUPZILLA_SRCDIR)/src/lib/cookies \
               $$(QUPZILLA_SRCDIR)/src/lib/downloads \
               $$(QUPZILLA_SRCDIR)/src/lib/history \
               $$(QUPZILLA_SRCDIR)/src/lib/navigation \
               $$(QUPZILLA_SRCDIR)/src/lib/network \
               $$(QUPZILLA_SRCDIR)/src/lib/notifications \
               $$(QUPZILLA_SRCDIR)/src/lib/opensearch \
               $$(QUPZILLA_SRCDIR)/src/lib/other \
               $$(QUPZILLA_SRCDIR)/src/lib/plugins \
               $$(QUPZILLA_SRCDIR)/src/lib/popupwindow \
               $$(QUPZILLA_SRCDIR)/src/lib/preferences \
               $$(QUPZILLA_SRCDIR)/src/lib/rss \
               $$(QUPZILLA_SRCDIR)/src/lib/session \
               $$(QUPZILLA_SRCDIR)/src/lib/sidebar \
               $$(QUPZILLA_SRCDIR)/src/lib/tabwidget \
               $$(QUPZILLA_SRCDIR)/src/lib/tools \
               $$(QUPZILLA_SRCDIR)/src/lib/webkit \
               $$(QUPZILLA_SRCDIR)/src/lib/webtab \

DEPENDPATH += $$INCLUDEPATH \
              $$(QUPZILLA_SRCDIR)/src/lib/data \

# vim:ts=4 sw=4 sts=4 et:
