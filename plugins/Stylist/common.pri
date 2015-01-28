OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build
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
