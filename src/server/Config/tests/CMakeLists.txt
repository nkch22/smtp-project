# Unit tests for Config component
add_executable(config_tests
    ConfigTests.cpp
)

target_link_libraries(config_tests PRIVATE 
    config
    gtest 
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(config_tests) 