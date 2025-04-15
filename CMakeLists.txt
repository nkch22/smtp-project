cmake_minimum_required(VERSION 3.20)

project(smtp_server)

include(cmake/CompileOptions.cmake)
include(cmake/InstallThirdParty.cmake)

add_executable(smtp_server)
add_subdirectory(src)

add_subdirectory(tests)
