#include <cstdlib>
#include <print>

#include "smtp_connection.hpp"

int main() {

  boost::asio::io_context io_context{};
  connection::SMTPConnection connection{io_context};
  io_context.run();

  return EXIT_SUCCESS;
}
