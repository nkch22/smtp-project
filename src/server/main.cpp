#include <cstdlib>
#include <print>

#include "SMTP/Server.hpp"

/*
    TODO: ADD ENCRYPTION
          INTEGRATE THREAD POOL
          TIMEOUT CONNECTION
          LIMIT OF RECEIVED BYTES
          BETTER BUFFER OBJECTS FOR ASYNC
          ADD FLUSH BUFFERS
*/

int main() 
{
    try
    {
        auto io_context{std::make_shared<asio::io_context>()};
        auto ssl_context{std::make_shared<asio::ssl::context>(asio::ssl::context::tlsv13)};
        auto server{std::make_shared<SMTP::Server>(io_context, ssl_context, 587)};
        server->Start();
        io_context->run();
        server->Stop();
    }
    catch(const std::exception& error)
    {
        std::println("Error: {}", error.what());
    }
    catch(...)
    {
        std::println("Unknown Error thrown");
    }
    return EXIT_SUCCESS;
}