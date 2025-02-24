#pragma once

#include "SSL/ServerBase.hpp"
#include "Session.hpp"

/*
    TODO: ADD ENCRYPTION(PROBABLY DONE)
          INTEGRATE THREAD POOL
          TIMEOUT CONNECTION
          LIMIT OF RECEIVED BYTES
          BETTER BUFFER OBJECTS FOR ASYNC
          ADD FLUSH BUFFERS
*/

namespace SMTP
{

class Server : public SSL::ServerBase
{
public:
    using Port = asio::ip::port_type;
    Server(std::shared_ptr<asio::io_context> io_context, 
           std::shared_ptr<asio::ssl::context> ssl_context, 
           const Port port);
    ~Server() = default;
protected:
    std::shared_ptr<SSL::SessionBase> CreateSession() override;
    void OnAccepted() override;
    void OnStarted() override;
private:
    std::shared_ptr<asio::io_context> m_io_context;
    std::shared_ptr<asio::ssl::context> m_ssl_context;
};

}