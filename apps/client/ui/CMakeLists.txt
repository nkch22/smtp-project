qt_add_library(ui_lib STATIC
        MainWindow.h MainWindow.cpp
        ConfigurationWidget.h ConfigurationWidget.cpp
        MessageSendingWidget.h MessageSendingWidget.cpp)

target_include_directories(ui_lib PUBLIC .)
target_link_libraries(ui_lib PUBLIC Qt6::Core Qt6::Widgets PRIVATE client)