#include <cstdlib>
#include <print>

#include "SMTP/Server.hpp"

int main() 
{
    SMTP::ServerContext server_context{};
    server_context.hostname = "smtp_server.com";

    SMTP::Server server{server_context};
    server.Listen(25);
    server.Run();
    
    return EXIT_SUCCESS;
}