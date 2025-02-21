#include <cstdlib>
#include <print>

#include "SMTP/TCP/Server.hpp"

int main() 
{
    auto io_context{std::make_shared<asio::io_context>()};
    auto server{std::make_shared<SMTP::TCP::Server>(io_context, 25)};
    server->Start();
    io_context->run();
    return EXIT_SUCCESS;
}