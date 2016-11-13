include($$project_root/common.pri)
TEMPLATE = lib
CONFIG += staticlib

SOURCES = setting.cpp vector_setting.cpp color_setting.cpp logging.cpp
HEADERS = \ 
    setting.hpp \
    vector_setting.hpp \
    vector_setting.tcc \
    color_setting.hpp \
    logging.hpp \

