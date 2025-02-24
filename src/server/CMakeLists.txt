add_library(smtp STATIC)
add_subdirectory(SMTP)
target_link_libraries(smtp PUBLIC asio )
target_link_libraries(smtp PRIVATE OpenSSL::SSL OpenSSL::Crypto)

add_executable(smtp_server main.cpp)
add_custom_target(copy_certificates ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/tools/certificates
    ${CMAKE_CURRENT_BINARY_DIR}/tools/certificates
    COMMENT "Copying tls/ssl certificates into server binary directory")

add_dependencies(smtp_server copy_certificates)
target_link_libraries(smtp_server PRIVATE smtp)