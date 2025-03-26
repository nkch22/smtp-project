add_library(logger_shared SHARED "Include/Src/Logger.cpp" "Include/Src/TemplateWrapper.cpp" "Include/Src/Buffer.cpp"  "Include/Src/ThreadMap.cpp")
target_link_libraries(logger_shared PRIVATE concurrency)

add_executable(example "Example.cpp")
target_link_libraries(example logger_shared)

add_executable(config_example "ConfigExample.cpp")
target_link_libraries(config_example logger_shared)

add_executable(Main "Main.cpp")
target_link_libraries(Main PUBLIC logger_shared)