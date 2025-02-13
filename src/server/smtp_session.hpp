#pragma once

#include <memory>

#include <asio.hpp>

namespace connection {
class SMTPSession : public std::enable_shared_from_this<SMTPSession> {
 public:
  explicit SMTPSession(asio::ip::tcp::socket socket);
  void Run();

 private:
  void WaitForRequest();

  asio::ip::tcp::socket socket_;
  asio::streambuf buffer_;
};
}  // namespace connection