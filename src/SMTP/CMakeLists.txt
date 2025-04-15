file(GLOB smtp_src CONFIGURE_DEPENDS "*.c" "*.cpp")

add_library(smtp STATIC)

target_sources(smtp PRIVATE ${smtp_src})

add_subdirectory(Protocol)
add_subdirectory(SSL)

target_link_libraries(smtp PUBLIC asio::asio aklomp::base64)
target_link_libraries(smtp PRIVATE OpenSSL::SSL OpenSSL::Crypto)