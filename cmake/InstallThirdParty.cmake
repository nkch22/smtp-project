include(FetchContent)

# --------------------------------------------------------------------

# Offline mode (uncomment next line to enable)

# set(FETCHCONTENT_FULLY_DISCONNECTED ON)
# set(FETCHCONTENT_QUIET OFF)

# --------------------------------------------------------------------

# Libraries

# --------------------------------------------------------------------

project_log("FetchContent: boost")

find_package(Boost 1.86.0 COMPONENTS asio)
if(NOT Boost_FOUND)

set(BOOST_INCLUDE_LIBRARIES asio)
set(BOOST_ENABLE_CMAKE ON)

FetchContent_Declare(Boost
GIT_REPOSITORY https://github.com/boostorg/boost.git
GIT_TAG boost-1.86.0
GIT_SHALLOW TRUE
)
FetchContent_MakeAvailable(Boost)
endif()