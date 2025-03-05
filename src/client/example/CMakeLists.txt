add_executable(test_server Server.cpp)
target_link_libraries(test_server PUBLIC asio base64)

add_executable(test_client MainClient.cpp)
target_link_libraries(test_client PRIVATE client)