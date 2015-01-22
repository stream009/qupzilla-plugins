OBJECTS_DIR = build
MOC_DIR = build
RCC_DIR = build
PRECOMPILED_DIR = build

QMAKE_CXXFLAGS += -std=c++11
QMAKE_CXXFLAGS += -Wno-deprecated-register

CONFIG += precompile_header

pch.commands = grep -hr \"$$(LITERAL_HASH)include <\" --include=\"*.cpp\" \
		    --include=\"*.h\" --exclude-dir=test \
		    | sort | uniq > stable.h
QMAKE_EXTRA_TARGETS += pch
