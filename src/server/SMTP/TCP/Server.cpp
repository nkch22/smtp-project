#include "Server.hpp"

#include <print>

namespace SMTP
{

namespace TCP
{

Server::Server(std::shared_ptr<asio::io_context> io_context, const Port port)
    : m_io_context{}
    , m_started{false}
    , m_endpoint{asio::ip::tcp::v4(), port}
    , m_acceptor{*io_context}
    , m_sessions{}
{
}

void Server::Start() 
{
    if(IsStarted())
    {
        return;
    }

    auto self{shared_from_this()};
    auto start_handler{[this, self]
    {
        if(IsStarted())
        {
            return;
        }
        m_started = true;
        m_acceptor.open(m_endpoint.protocol());
        m_acceptor.set_option(asio::ip::tcp::acceptor::reuse_address{true});
        m_acceptor.bind(m_endpoint);
        m_acceptor.listen();

        OnStarted();
        Accept();
    }};
    start_handler();
    //m_io_context->post(start_handler);
}

void Server::Stop() 
{
    if(!IsStarted())
    {
        return;
    }

    auto self{shared_from_this()};
    auto stop_handler{[this, self]
    {
        if(!IsStarted())
        {
            return;
        }

        m_acceptor.close();
        DisconnectAll();
        m_started = false;
        m_sessions.clear();
        OnStopped();
    }};

    stop_handler();
    //m_io_context->post(stop_handler);
}

void Server::Restart() 
{
    if(!IsStarted())
    {
        return;
    }
    Stop();
    Start();
    OnRestarted();
}

bool Server::IsStarted() const 
{
    return m_started;
}

void Server::Accept()
{
    if(!IsStarted())
    {
        return;
    }

    auto self{shared_from_this()};
    auto accept_handler{[this, self]
    {
        if(!IsStarted())
        {
            return;
        }

    }};
    auto async_accept_handler{[this, self](const asio::error_code& error, asio::ip::tcp::socket socket)
    {
        if(!error)
        {
            auto session{CreateSession(std::move(socket))};
            RegisterSession(session);

            session->Connect();
        }
        else
        {
            HandleError(error);
        }

        Accept();
    }};
    //accept_handler();
    m_acceptor.async_accept(async_accept_handler);
    //m_io_context->dispatch(accept_handler);
}

std::shared_ptr<Session> Server::CreateSession(asio::ip::tcp::socket socket)
{
    return std::make_shared<Session>(m_io_context, std::move(socket));
}

void Server::DisconnectAll()
{
    if(!IsStarted())
    {
        return;
    }

    auto self{shared_from_this()};
    auto disconnect_all_handler{[this, self]
    {
        if(!IsStarted())
        {
            return;
        }

        std::shared_lock<std::shared_mutex> lock{m_sessions_mutex};
        for(auto& session : m_sessions) 
        {
            session->Disconnect();
        }
    }};
    disconnect_all_handler();
    //m_io_context->dispatch(disconnect_all_handler);
}

void Server::HandleError(const asio::error_code& error)
{
    std::println("Error: {}", error.message());
}

void Server::RegisterSession(std::shared_ptr<ISession> session)
{
    m_sessions.insert(session);
}

void Server::UnregisterSession(std::shared_ptr<ISession> session)
{
    m_sessions.erase(session);
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