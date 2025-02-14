set(lib logger)
set(exe log_main)

add_library(${lib} "Logger.cpp")
add_executable(${exe} "Main.cpp")