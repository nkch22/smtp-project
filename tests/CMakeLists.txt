include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

set(TEST_SOURCES
    MainTests.cpp
    server/JsonParserTests.cpp
    server/ConfigTests.cpp
)

add_library(json_parser STATIC
    ${CMAKE_SOURCE_DIR}/src/server/JsonParser/JsonParser.cpp
)
target_include_directories(json_parser PUBLIC ${CMAKE_SOURCE_DIR}/src)

add_library(config STATIC
    ${CMAKE_SOURCE_DIR}/src/server/Config/Config.cpp
)
target_include_directories(config PUBLIC ${CMAKE_SOURCE_DIR}/src)

add_executable(tests_executable ${TEST_SOURCES})
target_link_libraries(tests_executable PRIVATE json_parser config gtest gtest_main)

include(GoogleTest)
gtest_discover_tests(tests_executable)
