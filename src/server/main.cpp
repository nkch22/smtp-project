#include <cstdlib>
#include <print>

#include "smtp_connection.hpp"

int main() {
  connection::SMTPConnection connection{};

  std::println("{}", connection.Read());
  return EXIT_SUCCESS;
}
