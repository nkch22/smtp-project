#include <cstdlib>
#include <print>

#include "connection/smtp_connection.hpp"

int main() {
 
  asio::io_context io_context{};
  connection::SMTPConnection connection{io_context};

  io_context.run();

  return EXIT_SUCCESS;
}
