#pragma once

#include <boost/asio.hpp>
#include <string>

namespace connection {
class SMTPConnection {
 public:
  using port_type = boost::asio::ip::port_type;

  explicit SMTPConnection(boost::asio::io_context& io_context, const port_type port_num = 25);
 private:
  void DoAccept();
  boost::asio::ip::tcp::acceptor acceptor_;
};
}  // namespace connection
