#pragma once

#include <list>
#include <vector>
#include <print>
#include <iostream>
#include <initializer_list>

#include <asio.hpp>

#include "ServerContext.hpp"
#include "Session.hpp"

namespace SMTP
{
   class Server
    {
    private:
        using SessionHandler = std::list<Session>::iterator;
        
    public:
        using Port = asio::ip::port_type;

        Server(const ServerContext context);

        void Listen(const Port port);
        void Run();

        const ServerContext& get_server_context() const;

    private:
        void HandleRead(SessionHandler connection_handler, const asio::error_code& error, 
                        const std::size_t bytes_transfered);
        void HandleWrite(SessionHandler connection_handler, std::shared_ptr<std::string> msg_buffer, 
                         const asio::error_code& err);
        void DoAsyncRead(SessionHandler connection_handler);
        void HandleAccept(SessionHandler connection_handler, const asio::error_code& error);
        void StartAccept();

        void SendResponse(SessionHandler connection_handler, std::string response);
        std::string ReceiveRequest(SessionHandler connection_handler);

        asio::io_service m_io_service;
        asio::ip::tcp::acceptor m_acceptor;
        std::list<Session> m_sessions;
        ServerContext m_server_context;
    }; 
}