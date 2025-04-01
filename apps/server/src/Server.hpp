#pragma once

#include "SMTP/Server.hpp"
#include "Logger/Include/Logger.h"

class Server final : public SMTP::Server
{
public:
    using Port = SMTP::Server::Port;
    using Context = SMTP::Context;
    using ContextGenerator = SMTP::Server::ContextGenerator;

    Server(std::shared_ptr<asio::io_context> io_context, 
           std::shared_ptr<asio::ssl::context> ssl_context, 
           ContextGenerator context_generator,
           const Port port);
    ~Server() = default;

    void OnStarted() override;
    void OnStopped() override;
    void OnRestarted() override;
    void OnAccepted() override;
private:
    std::shared_ptr<SMTP::SSL::SessionBase> CreateSession() override;
    logger::Logger log;
};