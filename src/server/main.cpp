#include <cstdlib>
#include <print>

#include "SMTP/Server.hpp"

int main() 
{
    auto io_context{std::make_shared<asio::io_context>()};
    auto server{std::make_shared<SMTP::Server>(io_context, 25)};
    server->Start();
    io_context->run();
    server->Stop();
    return EXIT_SUCCESS;
}