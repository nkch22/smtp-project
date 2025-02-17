function(find_src_files found_files_name)
endfunction()

add_library(smtp)
add_subdirectory(SMTP)
target_link_libraries(smtp PUBLIC asio)
target_include_directories(smtp PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<INSTALL_INTERFACE:SMTP>
)

add_executable(smtp_server main.cpp)
target_link_libraries(smtp_server PRIVATE smtp)
target_link_libraries(smtp_server PRIVATE crypto ssl)