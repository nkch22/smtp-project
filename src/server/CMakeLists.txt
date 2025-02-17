add_library(smtp_library)
add_subdirectory(SMTP)
target_link_libraries(smtp_library PUBLIC asio)

add_executable(smtp_server main.cpp)
target_link_libraries(smtp_server PRIVATE smtp_library)
target_link_libraries(smtp_server PRIVATE crypto ssl)