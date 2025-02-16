#include "Protocol/HeloCommand.hpp"

#include "Server.hpp"

namespace SMTP
{
    Server::Server() 
        : m_io_service{}, m_acceptor{m_io_service}
        , m_connections{}, m_hostname{}
    {
    }

    void Server::set_host(const std::string& hostname)
    {
        m_hostname = hostname;
    }

    void Server::set_port(const Port port) 
    {
        const asio::ip::tcp::endpoint endpoint{asio::ip::tcp::v4(), port};
        m_acceptor.open(endpoint.protocol());
        m_acceptor.set_option(asio::ip::tcp::acceptor::reuse_address{true});
        m_acceptor.bind(endpoint);
        m_acceptor.listen();
        StartAccept();
    }

    void Server::Run() 
    {
        m_io_service.run();
    }      

    void Server::HandleRead(ConnectionHandler connection, const asio::error_code& err, const std::size_t bytes_transfered) 
    {
        if(bytes_transfered > 0) 
        {
            if(connection->socket.is_open())
            {
                // Read completed successfully and connection is open
                SendResponse(connection, ReceiveRequest(connection)); 
            }
        }

        if(!err) 
        {
            DoAsyncRead(connection);
        } 
        else 
        {
            std::println(std::cerr, "Error: {}", err.message());
            m_connections.erase(connection);
        }
    }

    void Server::HandleWrite(ConnectionHandler connection, std::shared_ptr<std::string> /*msg_buffer*/, const asio::error_code& err) 
    {
        if(!err) 
        {
            if(connection->socket.is_open())
            {
                // Write completed successfully and connection is open
            }
        } 
        else 
        {
            std::println(std::cerr, "Error: {}", err.message());
            m_connections.erase(connection);
        }
    }

    void Server::DoAsyncRead(ConnectionHandler connection) 
    {
        auto handler{std::bind(&Server::HandleRead, this, connection, 
                                asio::placeholders::error, asio::placeholders::bytes_transferred)};
        asio::async_read_until(connection->socket, connection->read_buffer, "\n", handler);
    }

    void Server::HandleAccept(ConnectionHandler connection, asio::error_code const & err) 
    {
        if(!err) 
        {
            std::println("Connection from: {}", connection->socket.remote_endpoint().address().to_string());
            
            DoAsyncRead(connection);
        } 
        else 
        {
            std::println(std::cerr, "Error: {}", err.message());
            m_connections.erase(connection);
        }
        StartAccept();
    }

    void Server::StartAccept() 
    {
        auto connection{m_connections.emplace(m_connections.begin(), m_io_service)};
        auto handler{std::bind(&Server::HandleAccept, this, connection, asio::placeholders::error)};
        m_acceptor.async_accept(connection->socket, handler);
    }

    void Server::SendResponse(ConnectionHandler connection, std::string response)
    {
        auto buff{std::make_shared<std::string>(response)};
        auto handler{std::bind(&Server::HandleWrite, this, connection, buff, asio::placeholders::error)};
        asio::async_write(connection->socket, asio::buffer(*buff), handler);
    }

    std::string Server::ReceiveRequest(ConnectionHandler connection)
    {
        std::istream is{&connection->read_buffer};
        std::string line{};
        std::getline(is, line);
        std::println("Message Received: {}", line);
        return line;
    }
}