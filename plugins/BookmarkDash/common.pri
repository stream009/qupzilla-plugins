OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build
UI_DIR = build
PRECOMPILED_DIR = build

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -Wno-deprecated-register

CONFIG += precompile_header
PRECOMPILED_HEADER = stable.h

stable.target = stable.h
stable.commands = grep -hr \"$${LITERAL_HASH}include <\" --include=\"*.cpp\" \
		    --include=\"*.h\" --exclude-dir=test \
		    | sort | uniq > stable.h
QMAKE_EXTRA_TARGETS += stable

PRE_TARGETDEPS += stable.h
QMAKE_CLEAN += stable.h

INCLUDEPATH += $$project_root \
               $${project_root}/qt_mixin/include \

DEPENDPATH += $$project_root \
              $${project_root}/qt_mixin/include \

LIBS += -L$$project_root/plugin \
        -L$$project_root/common \
        -L$$project_root/bookmark \
        -L$${project_root}/qt_mixin/lib \

# vim:ts=4 sw=4 sts=4 et:
