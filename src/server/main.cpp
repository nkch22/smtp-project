#include <cstdlib>
#include <print>

#include "SMTP/Server.hpp"

int main() 
{
    SMTP::Server server{};
    server.set_host("smtp.gmail.com");
    server.set_port(25);
    server.Run();
    return EXIT_SUCCESS;
}
