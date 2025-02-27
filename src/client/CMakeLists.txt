cmake_minimum_required(VERSION 3.30)
project(smtp_client LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
qt_standard_project_setup()

qt_add_library(ui_lib STATIC
        ui/MainWindow.h ui/MainWindow.cpp
        ui/ConfigurationWidget.h ui/ConfigurationWidget.cpp
        ui/MessageSendingWidget.h ui/MessageSendingWidget.cpp)

target_include_directories(ui_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/ui)

target_link_libraries(ui_lib PUBLIC Qt6::Core Qt6::Widgets)

qt_add_executable(cmtp_client main.cpp)

target_link_libraries(cmtp_client PRIVATE ui_lib)

include_directories(include)
add_subdirectory(example)

add_library(client STATIC)
add_subdirectory(source)
target_link_libraries(client PUBLIC asio base64)
target_link_libraries(client PRIVATE OpenSSL::SSL OpenSSL::Crypto)