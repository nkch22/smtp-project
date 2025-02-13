#pragma once

#include <boost/asio.hpp>
#include <string>

namespace connection {
class SMTPConnection {
 public:
  using port_type = boost::asio::ip::port_type;

  explicit SMTPConnection(const port_type port_num = 25);
  std::string Read();
  void Send(const std::string& message);

 private:
  boost::asio::io_service io_service_;
  boost::asio::ip::tcp::acceptor acceptor_;
  boost::asio::ip::tcp::socket socket_;
};
}  // namespace connection
