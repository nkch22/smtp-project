#include "ServerBase.hpp"

#include <print>

namespace SMTP
{

namespace SSL
{

ServerBase::ServerBase(std::shared_ptr<asio::io_context> io_context,
                       std::shared_ptr<asio::ssl::context> ssl_context,
                       const ServerOptions server_options,
                       const Port port)
    : m_io_context{io_context}
    , m_ssl_context{ssl_context}
    , m_started{false}
    , m_acceptor{*m_io_context}
    , m_endpoint{asio::ip::tcp::v4(), port}
    , m_sessions{}
    , m_sessions_mutex{}
    , m_server_options{server_options}
{
}

std::shared_ptr<SSL::SessionBase> ServerBase::CreateSession()
{
    return std::make_shared<SessionBase>(m_io_context, m_ssl_context);
}

void ServerBase::Start()
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
    m_io_context->post(start_handler);
}

void ServerBase::Stop()
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
    m_io_context->post(stop_handler);
}

void ServerBase::Restart()
{
    if(!IsStarted())
    {
        return;
    }
    Stop();
    Start();
    OnRestarted();
}

void ServerBase::Accept()
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

        auto session{CreateSession()};
        auto async_accept_handler{[this, self, session](const asio::error_code& error)
        {
            if(!error)
            {
                OnAccepted();

                session->Connect();
            }
            else
            {
                HandleError(error);
            }
    
            Accept();
        }};
        m_acceptor.async_accept(session->get_socket(), async_accept_handler);
    }};
    m_io_context->dispatch(accept_handler);
}

bool ServerBase::IsStarted() const noexcept
{
    return m_started;
}

void ServerBase::HandleError(const asio::error_code& error)
{
    std::println("Error: {}", error.message());
}

void ServerBase::DisconnectAll()
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
    m_io_context->dispatch(disconnect_all_handler);
}

bool ServerBase::Multicast(const std::string_view data)
{
    if(!IsStarted())
    {
        return false;
    }
    if(std::size(data) == 0)
    {
        return true;
    }
    std::shared_lock<std::shared_mutex> lock{m_sessions_mutex};
    for(auto& session : m_sessions)
    {
        session->Send(data);
    }
    return true;
}

void ServerBase::OnStarted()
{
}

void ServerBase::OnStopped()
{
}

void ServerBase::OnRestarted()
{
}

void ServerBase::OnAccepted()
{
}

}

}