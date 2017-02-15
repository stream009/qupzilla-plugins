include($$project_root/common.pri)

TARGET = DownloadDash

SOURCES += plugin.cpp
HEADERS += plugin.hpp

PLUGIN_DIR = $$PWD
srcdir = $$(QUPZILLA_SRCDIR)
equals(srcdir, "") {
    include(../../plugins.pri)
}
else {
    include($$srcdir/src/plugins.pri)
}
