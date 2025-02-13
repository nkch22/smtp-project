#pragma once

#include <boost/asio.hpp>
#include <memory>

namespace connection {
class SMTPSession : public std::enable_shared_from_this<SMTPSession> {
 public:
  explicit SMTPSession(boost::asio::ip::tcp::socket socket);
  void Run();

 private:
  void WaitForRequest();

  boost::asio::ip::tcp::socket socket_;
  boost::asio::streambuf buffer_;
};
}  // namespace connection