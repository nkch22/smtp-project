set(lib logger)
set(exe log_main)

add_library(${lib} SHARED "Logger.cpp")

add_executable(${exe} "Main.cpp")

target_link_libraries(${exe} ${lib})