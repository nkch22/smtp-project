#pragma once

#include "Protocol/Parser.hpp"
#include "SSL/SessionBase.hpp"
#include "Protocol/Parser.hpp"

namespace SMTP
{

class Session : public SSL::SessionBase
{
public:
    Session(std::shared_ptr<asio::io_context> io_context, 
            std::shared_ptr<asio::ssl::context> ssl_context, 
            std::shared_ptr<Protocol::Parser> smtp_parser);
    ~Session() = default;
protected:
    void OnConnected() override;
    void OnDisconnected() override;
    void OnReceived(const std::string_view data) override;
    void OnSent(const std::size_t sent) override;
    void OnHandshaked() override;
private:
    std::shared_ptr<Protocol::Parser> m_smtp_parser;
};

}