OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build
PRECOMPILED_DIR = build

clang:QMAKE_CXX = clang++
QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -Wall -Wextra
QMAKE_CXXFLAGS += -Werror -Wfatal-errors
QMAKE_CXXFLAGS += -fvisibility=hidden
clang:QMAKE_CXXFLAGS += -fcolor-diagnostics
clang:QMAKE_CXXFLAGS += -Wno-inconsistent-missing-override

CONFIG += precompile_header
PRECOMPILED_HEADER = stable.h

stable.target = stable.h
stable.commands = grep -hr \"$${LITERAL_HASH}include <\" --include=\"*.cpp\" \
		    --include=\"*.h\" --exclude-dir=test \
		    | sort | uniq > stable.h
QMAKE_EXTRA_TARGETS += stable

PRE_TARGETDEPS += stable.h
QMAKE_CLEAN += stable.h

INCLUDEPATH += $$project_root
DEPENDPATH += $$project_root

LIBS += -L$$project_root/core \
        -L$$project_root/css \
        -L$$project_root/common \
        -L$$project_root/gui \
        -L$$project_root/plugin
