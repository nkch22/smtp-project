#include <print>
#include <iostream>

#include "smtp_session.hpp"

namespace connection
{
    SMTPSession::SMTPSession(asio::ip::tcp::socket socket)
        : socket_{std::move(socket)}
    {
    }

    void SMTPSession::Run()
    {
        WaitForRequest();
    }

    void SMTPSession::WaitForRequest()
    {
        auto self{shared_from_this()};

        asio::async_read_until(socket_, buffer_, "\0", [this, self](asio::error_code error, std::size_t)
        {
            if(!error)
            {
                std::string data{std::istreambuf_iterator<char>{&buffer_}, std::istreambuf_iterator<char>{}};

                std::println("{}", data);
                WaitForRequest();
            }
            else
            {
                std::println(std::cerr, "Error: {}", error.message());
            }
        });
    }
}
