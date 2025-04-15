find_package(asio 1.32.0 REQUIRED)
find_package(OpenSSL 3.1.3 REQUIRED)
find_package(spdlog 1.15.1 REQUIRED)
find_package(nlohmann_json 3.11.3 REQUIRED)
find_package(base64 REQUIRED)
find_package(GTest REQUIRED)

include(FetchContent)
FetchContent_Declare(
        pqxx
        GIT_REPOSITORY https://github.com/jtv/libpqxx.git
        GIT_TAG 7.10.0
)
FetchContent_MakeAvailable(pqxx)
include_directories(${pqxx_SOURCE_DIR}/include ${pqxx_BINARY_DIR}/include)