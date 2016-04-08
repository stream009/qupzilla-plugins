TEMPLATE = subdirs
SUBDIRS = plugin common bookmark

plugin.depends = common bookmark
bookmark.depends = common

# vim:ts=4 sw=4 sts=4 et:
