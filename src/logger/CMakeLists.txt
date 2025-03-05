
add_library(logger SHARED "Logger.cpp")

add_executable(example "Example.cpp")

target_link_libraries(example logger)