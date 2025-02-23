include_directories(include)
include_directories(example)

add_library(client STATIC)
add_subdirectory(source)
target_link_libraries(client PUBLIC asio base64)
target_link_libraries(client PRIVATE OpenSSL::SSL OpenSSL::Crypto)