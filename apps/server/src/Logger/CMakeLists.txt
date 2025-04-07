add_library(logger_shared SHARED "Include/Src/Logger.cpp" "Include/Src/TemplateWrapper.cpp" "Include/Src/Buffer.cpp"  "Include/Src/ThreadMap.cpp"  "Include/Src/LogLevels.cpp" "Include/Src/RealLogger.cpp"  "Include/Src/MessageType.cpp")
target_link_libraries(logger_shared PRIVATE concurrency)

add_executable(logger_example "Example.cpp")
target_link_libraries(logger_example logger_shared)

add_executable(logger_config_example "ConfigExample.cpp")
target_link_libraries(logger_config_example logger_shared)
