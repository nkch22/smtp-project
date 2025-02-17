include(FetchContent)

# --------------------------------------------------------------------

# Offline mode (uncomment next line to enable)

# set(FETCHCONTENT_FULLY_DISCONNECTED ON)
# set(FETCHCONTENT_QUIET OFF)

# --------------------------------------------------------------------

# Libraries

# --------------------------------------------------------------------

project_log("FetchContent: openssl")
FetchContent_Declare(
  ssl_content
  URL      https://github.com/janbar/openssl-cmake/archive/refs/tags/1.1.1k-20210430.tar.gz
  URL_HASH MD5=f4294e5de7ebb20f9a695155660f1afd
)
FetchContent_MakeAvailable(ssl_content)
add_library(OpenSSL::SSL ALIAS ssl)
add_library(OpenSSL::Crypto ALIAS crypto)

project_log("FetchContent: asio")
set(FETCHCONTENT_QUIET FALSE)
FetchContent_Declare(
        asio
        GIT_REPOSITORY https://github.com/chriskohlhoff/asio.git
        GIT_TAG asio-1-32-0
        GIT_PROGRESS TRUE
)
FetchContent_MakeAvailable(asio)

add_library(asio INTERFACE)
target_include_directories(asio INTERFACE ${asio_SOURCE_DIR}/asio/include)
