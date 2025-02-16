#include <cstdlib>
#include <print>

#include "SMTP/Server.hpp"

int main() 
{
    SMTP::Server server{};
    server.Listen(25);
    server.Run();
    return EXIT_SUCCESS;
}
