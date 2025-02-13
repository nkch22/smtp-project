#include "smtp_connection.hpp"

namespace connection {
SMTPConnection::SMTPConnection(const port_type port_num)
    : io_service_{},
      acceptor_{
          io_service_,
          boost::asio::ip::tcp::endpoint{boost::asio::ip::tcp::v4(), port_num}},
      socket_{io_service_} {
  acceptor_.accept(socket_);
}

std::string SMTPConnection::Read() {
  boost::asio::streambuf buffer{};
  boost::asio::read_until(socket_, buffer, "\n");
  const std::string data{
      boost::asio::buffer_cast<const char*>(std::data(buffer))};
  return data;
}

void SMTPConnection::Send(const std::string& message) {
  const auto msg{message + "\n"};
  boost::asio::write(socket_, boost::asio::buffer(msg));
}
}  // namespace connection