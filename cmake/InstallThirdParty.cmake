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

FetchContent_Declare(
        libpqxx
        GIT_REPOSITORY https://github.com/jtv/libpqxx.git
        GIT_TAG 7.10.0
)
set(PQXX_LIBRARIES pqxx_static)

set(PQXX_BUILD_TEST OFF)
set(PQXX_BUILD_SHARED OFF)  # Ensure static build is consistent
FetchContent_MakeAvailable(libpqxx)
include_directories(SYSTEM ${libpqxx_SOURCE_DIR}/include ${libpqxx_BINARY_DIR}/include)

