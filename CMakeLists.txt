cmake_minimum_required(VERSION 3.30)

project(smtp_proc)

include(cmake/Ccache.cmake)
include(cmake/CompileOptions.cmake)
include(cmake/Sanitize.cmake)
include(cmake/Development.cmake)
include(cmake/CheckCompiler.cmake)
include(cmake/Logging.cmake)
include(cmake/InstallThirdParty.cmake)

add_subdirectory(src)
add_subdirectory(tests)