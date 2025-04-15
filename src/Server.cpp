#include "Server.hpp"

#include "Session.hpp"
#include "Logger/Logger.hpp"

Server::Server(std::shared_ptr<asio::io_context> io_context, 
               std::shared_ptr<asio::ssl::context> ssl_context, 
               ContextGenerator context_generator,
               const Port port) :
	SMTP::Server{io_context, ssl_context, context_generator, port}
{
}

std::shared_ptr<SMTP::SSL::SessionBase> Server::CreateSession()
{
    return std::make_shared<Session>(m_io_context, m_ssl_context, 
                                     m_session_register, m_context_generator());
}

void Server::OnStarted()
{
    Logger::info("Server is Started!");
}

void Server::OnStopped()
{
    Logger::info("Server is Stopped!");
}

void Server::OnRestarted()
{
    Logger::info("Server is Restarted!");
}

void Server::OnAccepted()
{
    Logger::info("Server Accepts Connection!");
}