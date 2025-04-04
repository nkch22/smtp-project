#include "../Server.hpp"

#include "../Session.hpp"

Server::Server(std::shared_ptr<asio::io_context> io_context, 
               std::shared_ptr<asio::ssl::context> ssl_context, 
               ContextGenerator context_generator,
               const Port port) :
	SMTP::Server{io_context, ssl_context, context_generator, port}, log{LOGGER_IN_INIT_LIST}
{
}

std::shared_ptr<SMTP::SSL::SessionBase> Server::CreateSession()
{
    return std::make_shared<Session>(m_io_context, m_ssl_context, 
                                     m_session_register, m_context_generator());
}

void Server::OnStarted()
{
    log.log_message("Server is Started!");
}

void Server::OnStopped()
{
    log.log_message("Server is Stopped!");
}

void Server::OnRestarted()
{
    log.log_message("Server is Restarted!");
}

void Server::OnAccepted()
{
    log.log_message("Server Accepts Connection!");
}