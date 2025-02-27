set(lib logger)
set(exe log_main)

add_library(${lib} SHARED "Logger.cpp")

add_executable(${exe} "Main.cpp")

target_link_libraries(${exe} ${lib})

target_link_libraries(${lib} PUBLIC concurrency)
target_include_directories(${lib} PUBLIC ${CMAKE_SOURCE_DIR}/src/server/Concurrency/include/Concurrency)