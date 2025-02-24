#include <cstdlib>
#include <print>

#include "SMTP/Server.hpp"

int main() 
{
    try
    {
        auto io_context{std::make_shared<asio::io_context>()};
        auto ssl_context{std::make_shared<asio::ssl::context>(asio::ssl::context::tls)};
        ssl_context->set_password_callback([](const size_t max_length, const asio::ssl::context::password_purpose& purpose) -> std::string 
        {
            return "hello"; 
        });
        ssl_context->use_certificate_chain_file("tools/certificates/cert.pem");
        ssl_context->use_private_key_file("tools/certificates/key.pem", asio::ssl::context::file_format::pem);
        ssl_context->use_tmp_dh_file("tools/certificates/dhparam.pem");
        auto server{std::make_shared<SMTP::Server>(io_context, ssl_context, 465)};
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
        std::println("Unknown error thrown");
    }
    return EXIT_SUCCESS;
}