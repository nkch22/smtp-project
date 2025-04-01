include(GoogleTest)

file(GLOB_RECURSE CONCURRENCY_TEST_SOURCES "*.cpp")

add_executable(concurrency_tests ${CONCURRENCY_TEST_SOURCES})
target_link_libraries(concurrency_tests
        gtest
        gtest_main
        pthread
        concurrency
)

gtest_discover_tests(concurrency_tests)
