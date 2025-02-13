#pragma once

#include <asio.hpp>
#include <string>

namespace connection {
class SMTPConnection {
 public:
  using port_type = asio::ip::port_type;

  explicit SMTPConnection(asio::io_context& io_context,
                          const port_type port_num = 25);

 private:
  void DoAccept();
  asio::ip::tcp::acceptor acceptor_;
};
}  // namespace connection
