include(FetchContent)

# --------------------------------------------------------------------

# Offline mode (uncomment next line to enable)

# set(FETCHCONTENT_FULLY_DISCONNECTED ON)
# set(FETCHCONTENT_QUIET OFF)

# --------------------------------------------------------------------

# Libraries

# --------------------------------------------------------------------

find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

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