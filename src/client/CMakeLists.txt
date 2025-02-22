# Find OpenSSL
find_package(OpenSSL REQUIRED)
include_directories(${OPENSSL_INCLUDE_DIR})

add_library(client_lib STATIC Client.cpp Socket.cpp SSLSocket.cpp Authenticator.cpp)
target_link_libraries(client_lib PUBLIC asio OpenSSL::SSL OpenSSL::Crypto base64)

add_executable(test_server Server.cpp)
target_link_libraries(test_server PUBLIC asio base64)

add_executable(client MainClient.cpp)
target_link_libraries(client PRIVATE client_lib)