#include <cstdlib>
#include <print>

#include "SMTP/Server.hpp"

/*
    TODO: ADD ENCRYPTION
          INTEGRATE THREAD POOL
          TIMEOUT CONNECTION
          LIMIT OF RECEIVED BYTES
          BETTER BUFFER OBJECTS FOR ASYNC
*/

int main() 
{
    auto io_context{std::make_shared<asio::io_context>()};
    auto server{std::make_shared<SMTP::Server>(io_context, 25)};//should be 587
    server->Start();
    io_context->run();
    server->Stop();
    return EXIT_SUCCESS;
}