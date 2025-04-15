#pragma once

#include <functional>

#include "Session.hpp"
#include "SSL/ServerBase.hpp"
#include "Protocol/Parser.hpp"

namespace SMTP
{

class Server : public SSL::ServerBase
{
public:
    using Port = asio::ip::port_type;
    using ContextGenerator = std::function<std::shared_ptr<Context>()>;

    Server(std::shared_ptr<asio::io_context> io_context, 
           std::shared_ptr<asio::ssl::context> ssl_context, 
           ContextGenerator context_generator,
           const Port port);
    ~Server() = default;
protected:
    std::shared_ptr<SSL::SessionBase> CreateSession() override;
    ContextGenerator m_context_generator;
};

}