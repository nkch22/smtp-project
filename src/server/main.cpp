#include <cstdlib>
#include <print>

#include "SMTP/Server.hpp"

int main() 
{
    SMTP::Server server{};
    server.SetHost("smtp.gmail.com");
    server.SetPort(25);
    server.Run();
    return EXIT_SUCCESS;
}
