#include <cassert>

#include "Server.hpp"

#include "Protocol/Command/ServiceReadyCommand.hpp"
#include "Protocol/Parser.hpp"

namespace SMTP
{
    Server::Server(const ServerContext server_context) 
        : m_io_service{}, m_acceptor{m_io_service}
        , m_sessions{}, m_server_context{server_context}
    {
    }

    void Server::Listen(const Port port) 
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

    const ServerContext& Server::get_server_context() const
    {
        return m_server_context;
    }

    void Server::HandleRead(SessionHandler connection, const asio::error_code& error, const std::size_t bytes_transfered) 
    {
        if(bytes_transfered > 0) 
        {
            if(connection->socket.is_open())
            {
                // Read completed successfully and connection is open
                Protocol::Parser parser{};
                auto command{parser.TryParseRequest(ReceiveRequest(connection))};
                if(command.has_value())
                {
                    SendResponse(connection, 
                        command.value()->CreateResponse(connection->session_context).CreateStringResponse());
                }
            }
        }

        if(!error) 
        {
            DoAsyncRead(connection);
        } 
        else 
        {
            std::println(std::cerr, "Error: {}", error.message());
            m_sessions.erase(connection);
        }
    }

    void Server::HandleWrite(SessionHandler connection, std::shared_ptr<std::string> msg_buffer, const asio::error_code& err) 
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
            m_sessions.erase(connection);
        }
    }

    void Server::DoAsyncRead(SessionHandler connection) 
    {
        auto handler{std::bind(&Server::HandleRead, this, connection, 
                               asio::placeholders::error, asio::placeholders::bytes_transferred)};
        asio::async_read_until(connection->socket, connection->read_buffer, "\r\n", handler);
    }

    void Server::HandleAccept(SessionHandler connection, const asio::error_code& error) 
    {
        if(!error) 
        {
            std::println("Connection from: {}", connection->socket.remote_endpoint()
                .address().to_string());
            
            Protocol::ServiceReadyCommand service_ready_command{};
            SendResponse(connection, 
                service_ready_command.CreateResponse(connection->session_context).CreateStringResponse());
            DoAsyncRead(connection);
        } 
        else 
        {
            std::println(std::cerr, "Error: {}", error.message());
            m_sessions.erase(connection);
        }
        StartAccept();
    }

    void Server::StartAccept() 
    {
        auto connection{m_sessions.emplace(m_sessions.begin(), m_io_service)};
        connection->session_context.hostname = get_server_context().hostname;
        auto handler{std::bind(&Server::HandleAccept, this, connection, asio::placeholders::error)};
        m_acceptor.async_accept(connection->socket, handler);
    }

    void Server::SendResponse(SessionHandler connection, std::string response)
    {
        auto buff{std::make_shared<std::string>(response)};
        auto handler{std::bind(&Server::HandleWrite, this, connection, buff, asio::placeholders::error)};
        asio::async_write(connection->socket, asio::buffer(*buff), handler);
    }

    std::string Server::ReceiveRequest(SessionHandler connection)
    {
        std::istream is{&connection->read_buffer};
        std::string line{};
        std::getline(is, line);
        std::println("Message Received: {}", line);
        return line;
    } 
}