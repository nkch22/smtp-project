add_library(smtp STATIC)
add_subdirectory(SMTP)
target_link_libraries(smtp PUBLIC asio::asio)
target_link_libraries(smtp PRIVATE OpenSSL::SSL)
target_link_libraries(smtp PRIVATE OpenSSL::Crypto)

add_executable(smtp_server main.cpp)
target_link_libraries(smtp_server PRIVATE smtp)