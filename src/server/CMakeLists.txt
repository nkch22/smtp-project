function(find_src_files found_files_name)
endfunction()

add_library(smtp STATIC)
add_subdirectory(SMTP)
target_link_libraries(smtp PUBLIC crypto ssl)
target_link_libraries(smtp PUBLIC asio)

add_executable(smtp_server main.cpp)
target_link_libraries(smtp_server PRIVATE smtp)