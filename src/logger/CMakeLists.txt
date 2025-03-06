
add_library(logger SHARED "Logger.cpp")

add_executable(example "Example.cpp")

target_link_libraries(example logger)

add_executable(con_test "Thr_test.cpp")
target_link_libraries(con_test logger)