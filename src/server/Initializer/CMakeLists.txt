add_executable(init_main "Main.cpp")

add_library(initializer "Initializer.cpp")
target_link_libraries(initializer PUBLIC logger_shared config)

target_link_libraries(init_main PUBLIC initializer)