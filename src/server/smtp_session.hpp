#pragma once

#include <memory>

#include <boost/asio.hpp>

namespace connection
{
    class SMTPSession : public std::enable_shared_from_this<SMTPSession>
    {
    public:
        SMTPSession(boost::asio::ip::tcp::socket socket);
        void Run();
    private:
        void WaitForRequest();

        boost::asio::ip::tcp::socket socket_;
        boost::asio::streambuf buffer_;
    };
}