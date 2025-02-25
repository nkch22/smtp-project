add_subdirectory(JSON)
add_subdirectory(Config)

add_executable(server main.cpp)

target_link_libraries(server
    PRIVATE
    json
    config
)
