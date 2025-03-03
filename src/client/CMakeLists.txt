cmake_minimum_required(VERSION 3.30)
project(smtp_client LANGUAGES CXX)

add_library(client STATIC)
target_include_directories(client PUBLIC include)
target_link_libraries(client PUBLIC asio base64 PRIVATE OpenSSL::SSL OpenSSL::Crypto)

add_subdirectory(example)
add_subdirectory(source)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
qt_standard_project_setup()

add_subdirectory(ui)

qt_add_executable(cmtp_client main.cpp)
target_link_libraries(cmtp_client PUBLIC client ui_lib Qt6::Core Qt6::Widgets)