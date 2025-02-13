add_library(connection_library)
add_subdirectory(connection)
target_link_libraries(connection_library PUBLIC asio)

add_executable(server main.cpp)
target_link_libraries(server PRIVATE connection_library)
