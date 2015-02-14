TEMPLATE = subdirs
QMAKE_SUBSTITUTES += .qmake.cache.in
SUBDIRS = plugin common

plugin.depends = common

# vim:ts=4 sw=4 sts=4 et:
