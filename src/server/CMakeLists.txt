add_subdirectory(JSON)
add_subdirectory(Config)
add_subdirectory(DB)

add_executable(server main.cpp)

target_link_libraries(server
    PRIVATE
    json
    config
    db
)