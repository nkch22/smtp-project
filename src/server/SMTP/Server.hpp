#pragma once

#include "SSL/ServerBase.hpp"
#include "Session.hpp"
#include "ServerOptions.hpp"

namespace SMTP
{

class Server : public SSL::ServerBase
{
public:
    using Port = asio::ip::port_type;
    Server(std::shared_ptr<asio::io_context> io_context, 
           std::shared_ptr<asio::ssl::context> ssl_context, 
           const ServerOptions server_options,
           const Port port);
    ~Server() = default;
protected:
    std::shared_ptr<SSL::SessionBase> CreateSession() override;
    void OnAccepted() override;
    void OnStarted() override;
};

}