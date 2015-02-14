TEMPLATE = subdirs
QMAKE_SUBSTITUTES += .qmake.cache.in
SUBDIRS = common \
          core \
          css \
          gui \
          plugin

core.depends = common css
css.depends = common
gui.depends = common core
plugin.depends = common core css gui

SUBDIRS += core_test \
           css_test

core_test.subdir = core/test
core_test.depends = core
css_test.subdir = css/test
css_test.depends = common css core

# vim:ts=4 sw=4 sts=4 et:
