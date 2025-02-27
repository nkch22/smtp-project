#pragma once

#include "Session.hpp"
#include "SSL/ServerBase.hpp"
#include "Protocol/Parser.hpp"

namespace SMTP
{

class Server : public SSL::ServerBase
{
public:
    using Port = asio::ip::port_type;
    Server(std::shared_ptr<asio::io_context> io_context, 
           std::shared_ptr<asio::ssl::context> ssl_context, 
           const Protocol::Options smtp_options,
           const Port port);
    ~Server() = default;
protected:
    std::shared_ptr<SSL::SessionBase> CreateSession() override;
    void OnAccepted() override;
    void OnStarted() override;

    std::shared_ptr<Protocol::Parser> m_smtp_parser;
};

}