include(common.pri)
TEMPLATE = subdirs
SUBDIRS = core dialog plugin

plugin.depends = core dialog
dialog.depends = core
