
add_library(logger SHARED "Include/Src/Logger.cpp")

add_executable(example "Example.cpp")
target_link_libraries(example logger)

add_executable(config_example "ConfigExample.cpp")
target_link_libraries(config_example logger)