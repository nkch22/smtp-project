add_executable(logger_tests "LoggerTests.cpp")
target_link_libraries(logger_tests logger_shared gtest gtest_main)