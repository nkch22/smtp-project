#pragma once

#include <list>
#include <print>
#include <iostream>

#include <asio.hpp>

#include "Connection.hpp"

namespace SMTP
{
    class Server
    {
    private:
        using ConnectionHandler = std::list<Connection>::iterator;
        
    public:
        using Port = asio::ip::port_type;

        Server();

        void SetHost(const std::string& hostname);
        void SetPort(const Port port);
        void Run();

    private:
        void HandleRead(ConnectionHandler connection_handler, const asio::error_code& err, 
                        const std::size_t bytes_transfered);
        void HandleWrite(ConnectionHandler connection_handler, std::shared_ptr<std::string> /*msg_buffer*/, 
                         const asio::error_code& err);
        void DoAsyncRead(ConnectionHandler connection_handler);
        void HandleAccept(ConnectionHandler connection_handler, asio::error_code const & err);
        void StartAccept();

        void SendResponse(ConnectionHandler connection_handler, std::string response);
        std::string ReceiveRequest(ConnectionHandler connection_handler);

        asio::io_service m_io_service;
        asio::ip::tcp::acceptor m_acceptor;
        std::list<Connection> m_connections;
        std::string m_hostname;
    };
}