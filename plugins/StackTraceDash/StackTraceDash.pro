include(common.pri)
TEMPLATE = subdirs
SUBDIRS = core plugin

plugin.depends = core
