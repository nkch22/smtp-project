#pragma once

#include <unordered_set>
#include <shared_mutex>
#include <memory>

#include <asio.hpp>

#include "Session.hpp"
#include "../IServer.hpp"

namespace SMTP
{

namespace TCP
{

class Server 
    : public IServer
    , public std::enable_shared_from_this<Server>
{
public:
    using Port = asio::ip::port_type;

    Server(std::shared_ptr<asio::io_context> io_context, const Port port);
    ~Server() = default;
    void Start() override;
    void Stop() override;
    void Restart() override;
    
    bool IsStarted() const noexcept;
    
protected:
    void Accept() override;
    void OnStarted() override;
    void OnStopped() override;
    void OnRestarted() override;
    void OnAccepted() override;

    virtual std::shared_ptr<Session> CreateSession(asio::ip::tcp::socket socket);
private:
    void RegisterSession(std::shared_ptr<ISession> session);
    void UnregisterSession(std::shared_ptr<ISession> session);
    void DisconnectAll();
    void HandleError(const asio::error_code& error);

    std::shared_ptr<asio::io_context> m_io_context;
    bool m_started;
    asio::ip::tcp::endpoint m_endpoint;
    asio::ip::tcp::acceptor m_acceptor;
    std::unordered_set<std::shared_ptr<ISession>> m_sessions;
    std::shared_mutex m_sessions_mutex;
};

}

}