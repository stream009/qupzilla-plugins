OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build
UI_DIR = build
PRECOMPILED_DIR = build

#CONFIG += clang

clang:QMAKE_CXX = "/usr/bin/time clang"
QMAKE_CXXFLAGS += -std=c++11 -pedantic
#QMAKE_CXXFLAGS += -Wconversion -Wold-style-cast -Werror
QMAKE_CXXFLAGS += -fvisibility=hidden
clang:QMAKE_CXXFLAGS += -fcolor-diagnostics -Wno-inconsistent-missing-override

CONFIG += precompile_header
PRECOMPILED_HEADER = stable.h

stable.target = stable.h
stable.commands = grep -hr \"$${LITERAL_HASH}include <\" --include=\"*.cpp\" \
		    --include=\"*.h\" --include=\"*.hpp\" --include=\"*.tcc\" \
            --exclude-dir=test --exclude-dir=build \
		    | sort | uniq > stable.h
QMAKE_EXTRA_TARGETS += stable

PRE_TARGETDEPS += stable.h
QMAKE_CLEAN += stable.h

INCLUDEPATH += $$project_root
DEPENDPATH  += $$project_root

LIBS += -L$$project_root/plugin \
        -L$$project_root/common \
        -L$$project_root/core \
        -L$$project_root/gui \
