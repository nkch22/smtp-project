add_subdirectory(Config)
add_subdirectory(SMTP)

file(GLOB smtp_server_src CONFIGURE_DEPENDS "*.c" "*.cpp")

target_sources(smtp_server PRIVATE ${smtp_server_src})

target_link_libraries(smtp_server PRIVATE pqxx spdlog::spdlog)
target_link_libraries(smtp_server PRIVATE smtp config)