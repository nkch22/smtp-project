cmake_minimum_required(VERSION 3.30.0)
project(SMTP)

set(CMAKE_CXX_STANDARD 23)

find_package(GTest 1.15.2 CONFIG)
if(NOT GTest_FOUND)

include(FetchContent)
FetchContent_Declare(googletest
    GIT_REPOSITORY https://github.com/google/googletest.git
    GIT_TAG v1.15.2)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

endif()

find_package(Boost 1.86.0 COMPONENTS asio)
if(NOT Boost_FOUND)

set(BOOST_INCLUDE_LIBRARIES asio)
set(BOOST_ENABLE_CMAKE ON)

include(FetchContent)
FetchContent_Declare(Boost
    GIT_REPOSITORY https://github.com/boostorg/boost.git
    GIT_TAG boost-1.86.0
    GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(Boost)

endif()

add_subdirectory(src)
add_subdirectory(tests)