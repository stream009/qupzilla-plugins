include($$project_root/common.pri)
TEMPLATE = subdirs
SUBDIRS = common plugin core gui

plugin.depends = common core gui
gui.depends = core common
core.depends = common
