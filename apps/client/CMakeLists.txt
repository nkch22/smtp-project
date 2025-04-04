cmake_minimum_required(VERSION 3.30)
project(smtp_client LANGUAGES CXX)

include(cmake/Ccache.cmake)
include(cmake/CheckCompiler.cmake)
include(cmake/CompileOptions.cmake)
include(cmake/Sanitize.cmake)
include(cmake/Development.cmake)
include(cmake/Logging.cmake)
include(cmake/InstallThirdParty.cmake)

add_library(client STATIC)
target_include_directories(client PUBLIC include)
target_link_libraries(client PUBLIC asio base64 PRIVATE OpenSSL::SSL OpenSSL::Crypto)

add_subdirectory(example)
add_subdirectory(source)

find_package(Qt6 REQUIRED COMPONENTS Core Widgets)
qt_standard_project_setup()

add_subdirectory(ui)

add_custom_target(copy_certificates ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/tools
        ${CMAKE_CURRENT_BINARY_DIR}/tools
        COMMENT "Copying tls/ssl certificates into client binary directory")
add_dependencies(client copy_certificates)
qt_add_executable(smtp_client main.cpp)
target_link_libraries(smtp_client PUBLIC client ui_lib Qt6::Core Qt6::Widgets)