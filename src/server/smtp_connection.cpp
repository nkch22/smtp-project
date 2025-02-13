#include "smtp_connection.hpp"

#include <iostream>
#include <memory>
#include <print>

#include "smtp_session.hpp"

namespace connection {
SMTPConnection::SMTPConnection(asio::io_context& io_context,
                               const port_type port_num)
    : acceptor_{io_context, asio::ip::tcp::endpoint{
                                asio::ip::tcp::v4(), port_num}} {
  DoAccept();
}

void SMTPConnection::DoAccept() {
  acceptor_.async_accept([this](asio::error_code error,
                                asio::ip::tcp::socket socket) {
    if (!error) {
      const auto session_address{
          socket.remote_endpoint().address().to_string()};
      const auto session_port{socket.remote_endpoint().port()};
      std::println("Creating session on: {}:{}", session_address, session_port);
      std::make_shared<SMTPSession>(std::move(socket))->Run();
    } else {
      std::println(std::cerr, "Error: {}", error.message());
    }
    DoAccept();
  });
}
}  // namespace connection