# Unit tests for JSON component
add_executable(json_tests
    JSONTest.cpp
    JsonParserTests.cpp
    SerializerTests.cpp
)

target_link_libraries(json_tests PRIVATE 
    json
    gtest 
    gtest_main
)

include(GoogleTest)
gtest_discover_tests(json_tests) 
