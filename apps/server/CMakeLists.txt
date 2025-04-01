cmake_minimum_required(VERSION 3.20)

project(smtp_server)

set(CMAKE_CXX_STANDARD 20)

include(cmake/Ccache.cmake)
include(cmake/CheckCompiler.cmake)
include(cmake/Sanitize.cmake)
include(cmake/Development.cmake)
include(cmake/Logging.cmake)
include(cmake/InstallThirdParty.cmake)

add_subdirectory(src/JSON)
add_subdirectory(src/Config)
add_subdirectory(src/DB)
add_subdirectory(src/Initializer)
add_subdirectory(src/Logger)
add_subdirectory(src/Concurrency)

add_library(smtp STATIC)
add_subdirectory(src/SMTP)

target_link_libraries(smtp PUBLIC asio )
target_link_libraries(smtp PRIVATE OpenSSL::SSL OpenSSL::Crypto)

add_executable(smtp_server src/main.cpp src/Database.cpp src/Mail.cpp src/Server.cpp src/Session.cpp src/User.cpp)

add_custom_target(copy_certificates ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/tools
        ${CMAKE_CURRENT_BINARY_DIR}/tools
        COMMENT "Copying tls/ssl certificates into server binary directory")
add_dependencies(smtp_server copy_certificates)


add_custom_target(copy_configs ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/configs
    ${CMAKE_CURRENT_BINARY_DIR}/configs
    COMMENT "Copying configs into server binary directory")
add_dependencies(smtp_server copy_configs)


target_link_libraries(smtp_server
    PRIVATE
    smtp
    logger_shared
    json
    config
    initializer
)