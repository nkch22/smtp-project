include(FetchContent)
# --------------------------------------------------------------------

# Offline mode (uncomment next line to enable)

# set(FETCHCONTENT_FULLY_DISCONNECTED ON)
# set(FETCHCONTENT_QUIET OFF)

# --------------------------------------------------------------------

# Libraries

# --------------------------------------------------------------------
project_log("FetchContent: asio")
FetchContent_Declare(
        asio
        GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
        GIT_TAG asio-1-32-0
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(asio)

add_library(asio INTERFACE)
target_include_directories(asio INTERFACE ${asio_SOURCE_DIR}/asio/include)

project_log("FetchContent: base64")
FetchContent_Declare(
    base64
    GIT_REPOSITORY https://github.com/ReneNyffenegger/cpp-base64.git
    GIT_TAG master
)
FetchContent_MakeAvailable(base64)

add_library(base64 STATIC ${base64_SOURCE_DIR}/base64.cpp)
target_include_directories(base64 INTERFACE ${base64_SOURCE_DIR})

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

# TODO make fetch optional if we are not compiling any tests
project_log("FetchContent: googletest")
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/refs/tags/v1.15.2.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

