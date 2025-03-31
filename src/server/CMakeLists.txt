add_subdirectory(JSON)
add_subdirectory(Config)
add_subdirectory(DB)
add_subdirectory(Initializer)
add_subdirectory(Logger)
add_subdirectory(Concurrency)

add_library(smtp STATIC)
add_subdirectory(SMTP)
target_link_libraries(smtp PUBLIC asio )
target_link_libraries(smtp PRIVATE OpenSSL::SSL OpenSSL::Crypto)

add_executable(smtp_server main.cpp Database.cpp Mail.cpp Server.cpp Session.cpp User.cpp)

add_dependencies(smtp_server copy_certificates)
add_custom_target(copy_configs ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/configs
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