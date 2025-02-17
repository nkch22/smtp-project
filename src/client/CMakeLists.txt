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

target_link_libraries(cmtp_client PRIVATE Qt6::Core Qt6::Widgets)include_directories(include)

add_library(client STATIC)
add_subdirectory(source)
add_subdirectory(example)
target_link_libraries(client PUBLIC asio base64)
target_link_libraries(client PRIVATE OpenSSL::SSL OpenSSL::Crypto)