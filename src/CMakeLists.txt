add_custom_target(copy_certificates ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${PROJECT_SOURCE_DIR}/tools/certificates
        ${CMAKE_CURRENT_BINARY_DIR}/tools/certificates
        COMMENT "Copying tls/ssl certificates into server binary directory")

add_subdirectory(client)
add_subdirectory(server)