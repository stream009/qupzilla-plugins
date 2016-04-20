CONFIG += c++11

QMAKE_CXXFLAGS += -pedantic -Wall -Wextra
QMAKE_LFLAGS += -Wl,--no-undefined

INCLUDEPATH += $$project_root
DEPENDPATH += $$project_root
