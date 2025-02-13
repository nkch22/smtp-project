add_executable(server main.cpp smtp_connection.cpp smtp_session.cpp)
target_link_libraries(server PRIVATE Boost::asio)