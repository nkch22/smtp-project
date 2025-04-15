add_executable(smtp_test)
add_subdirectory(SMTP)
target_link_libraries(smtp_test gtest::gtest)

include(GoogleTest)
gtest_discover_tests(smtp_test)