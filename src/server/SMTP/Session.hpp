#pragma once

#include "Protocol/Parser.hpp"
#include "Utils.hpp"

namespace SMTP
{

class Session : public SessionBase
{
public:
    Session(std::shared_ptr<asio::io_context> io_context, asio::ip::tcp::socket socket);
    ~Session() = default;

protected:
    void OnConnected() override;
    void OnDisconnected() override;
    void OnReceived(const std::string_view data) override;
    void OnSent(const std::size_t sent) override;
private:
    Protocol::Parser m_request_parser;
};

}