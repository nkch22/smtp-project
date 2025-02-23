#pragma once

#include <memory>
#include <unordered_set>
#include <shared_mutex>

#include <asio.hpp>
#include <asio/ssl.hpp>

#include "../IServer.hpp"
#include "Session.hpp"

namespace SMTP
{

namespace SSL
{

class Server 
    : public IServer
    , public std::enable_shared_from_this<Server>
{
public:
    using Port = asio::ip::port_type;
    Server(std::shared_ptr<asio::io_context> io_context,
           std::shared_ptr<asio::ssl::context> ssl_context, 
           const Port port);
    ~Server() = default;
    void Start() override;
    void Stop() override;
    void Restart() override;
    void Accept() override;

    bool IsStarted() const noexcept;
protected:
    virtual std::shared_ptr<ISession> CreateSession();
    void OnStarted() override;
    void OnStopped() override;
    void OnRestarted() override;
    void OnAccepted() override;
private:
    std::shared_ptr<asio::io_context> m_io_context;
    std::shared_ptr<asio::ssl::context> m_ssl_context;
    std::atomic<bool> m_started;
    asio::ip::tcp::endpoint m_endpoint;
    asio::ip::tcp::acceptor m_acceptor;
    std::unordered_set<std::shared_ptr<ISession>> m_sessions;
    std::shared_mutex m_sessions_mutex;
};

}

}