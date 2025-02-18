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
        using ConnectionHandler = std::list<Session>::iterator;
        
    public:
        using Port = asio::ip::port_type;

        Server(const ServerContext context);

        void Listen(const Port port);
        void Run();

        const ServerContext& get_server_context() const;

    private:
        void HandleRead(ConnectionHandler connection_handler, const asio::error_code& err, 
                        const std::size_t bytes_transfered);
        void HandleWrite(ConnectionHandler connection_handler, std::shared_ptr<std::string> msg_buffer, 
                         const asio::error_code& err);
        void DoAsyncRead(ConnectionHandler connection_handler);
        void HandleAccept(ConnectionHandler connection_handler, asio::error_code const & err);
        void StartAccept();

        void SendResponse(ConnectionHandler connection_handler, std::string response);
        std::string ReceiveRequest(ConnectionHandler connection_handler);

        asio::io_service m_io_service;
        asio::ip::tcp::acceptor m_acceptor;
        std::list<Session> m_connections;
        ServerContext m_server_context;
    }; 
}