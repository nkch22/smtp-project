#include "Server.hpp"

namespace SMTP
{

namespace SSL
{

Server::Server(std::shared_ptr<asio::io_context> io_context,
               std::shared_ptr<asio::ssl::context> ssl_context,
               const Port port)
    : m_io_context{io_context}
    , m_ssl_context{ssl_context}
    , m_started{false}
    , m_sessions{}
    , m_sessions_mutex{}
    , m_endpoint{}
    , m_acceptor{*m_io_context, port}
{
}

std::shared_ptr<ISession> Server::CreateSession()
{
    return std::make_shared<Session>();
}

void Server::Start()
{
}

void Server::Stop()
{
}

void Server::Restart()
{
}

void Server::Accept()
{
}

bool Server::IsStarted() const noexcept
{
    return m_started;
}

void Server::OnStarted()
{
}

void Server::OnStopped()
{
}

void Server::OnRestarted()
{
}

void Server::OnAccepted()
{
}

}

}