include($$project_root/common.pri)
TEMPLATE = lib
CONFIG += staticlib
QT += widgets

QMAKE_CXXFLAGS += -g

INCLUDEPATH += $$project_root/lib

FORMS = \
    #setting_dialog.ui \
    #subscriptions_tab.ui \
    new_subscription_dialog.ui \ 
    #subscription_widget.ui \
    #left_panel.ui \
    #right_panel.ui \
    #find_bar.ui \
    #custom_list_tab.ui \
    #custom_list_widget.ui \

SOURCES = \
    setting_dialog.cpp \
    new_subscription_dialog.cpp \
    subscriptions_tab.cpp \
    subscriptions_model.cpp \
    subscriptions_view.cpp \
    subscriptions_viewer.cpp \
    subscription_widget.cpp \
    line_edit.cpp \
    filter_list_model.cpp \
    left_panel.cpp \
    right_panel.cpp \
    find_bar.cpp \
    filter_list_view.cpp \
    filter_list_viewer.cpp \
    filter_list_actions.cpp \
    custom_filter_sets_tab.cpp \
    custom_filter_sets_view.cpp \
    custom_filter_sets_viewer.cpp \
    custom_filter_sets_model.cpp \
    custom_filter_set_widget.cpp \
    filter_set_widget.cpp \
    filter_set_actions.cpp \
    filter_sets_actions.cpp \

HEADERS = \
    setting_dialog.hpp \
    new_subscription_dialog.hpp \
    subscriptions_tab.hpp \
    subscriptions_model.hpp \
    subscriptions_view.hpp \
    subscriptions_viewer.hpp \
    subscription_widget.hpp \
    line_edit.hpp \
    filter_list_model.hpp \
    left_panel.hpp \
    right_panel.hpp \
    find_bar.hpp \
    filter_list_view.hpp \
    filter_list_viewer.hpp \
    filter_list_actions.hpp \
    custom_filter_sets_tab.hpp \
    custom_filter_sets_view.hpp \
    custom_filter_sets_viewer.hpp \
    custom_filter_sets_model.hpp \
    custom_filter_set_widget.hpp \
    filter_set_widget.hpp \
    filter_set_actions.hpp \
    filter_sets_actions.hpp \
