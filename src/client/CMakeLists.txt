cmake_minimum_required(VERSION 3.30)
project(smtp_client LANGUAGES CXX)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
qt_standard_project_setup()

qt_add_executable(cmtp_client
        main.cpp
        ui/MainWindow.h
        ui/MainWindow.cpp
)

target_include_directories(cmtp_client PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/ui)

target_link_libraries(cmtp_client PRIVATE Qt6::Core Qt6::Widgets)