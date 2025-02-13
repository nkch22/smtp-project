#include <memory>
#include <print>
#include <iostream>

#include "smtp_session.hpp"
#include "smtp_connection.hpp"

namespace connection {
SMTPConnection::SMTPConnection(boost::asio::io_context& io_context, const port_type port_num)
    : 
      acceptor_{
          io_context,
          boost::asio::ip::tcp::endpoint{boost::asio::ip::tcp::v4(), port_num}}{
            DoAccept();
}

void SMTPConnection::DoAccept()
{
  acceptor_.async_accept([this](boost::system::error_code error, boost::asio::ip::tcp::socket socket)
  {
    if(!error)
    {
      const auto session_address{socket.remote_endpoint().address().to_string()};
      const auto session_port{socket.remote_endpoint().port()};
      std::println("Creating session on: {}:{}", session_address, session_port);

      std::make_shared<SMTPSession>(std::move(socket))->Run();
    }
    else
    {
      std::println(std::cerr, "Error: {}", error.message());
    }
    DoAccept();
  });
}
}  // namespace connection