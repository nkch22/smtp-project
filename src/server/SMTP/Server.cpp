#include "Server.hpp"

#include <print>

#include "Session.hpp"

namespace SMTP
{

Server::Server(std::shared_ptr<asio::io_context> io_context, 
               std::shared_ptr<asio::ssl::context> ssl_context, 
               std::function<std::shared_ptr<Context>()> context_generator,
               const Port port)
    : SSL::ServerBase{io_context, ssl_context, port}
    , m_context_generator{context_generator}
{
}

std::shared_ptr<SSL::SessionBase> Server::CreateSession()
{
    return std::make_shared<Session>(m_io_context, m_ssl_context, 
                                     m_session_register, m_context_generator());
}

void Server::OnAccepted()
{
    std::println("Accepting Connection");
}

void Server::OnStarted()
{
    std::println("Server is started");
}

}