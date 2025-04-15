#pragma once

#include <memory>
#include <unordered_set>
#include <shared_mutex>

#include <asio.hpp>
#include <asio/ssl.hpp>

#include "../IServer.hpp"
#include "SessionBase.hpp"
#include "SessionRegisterBase.hpp"

namespace SMTP
{

namespace SSL
{

class ServerBase 
    : public IServer
    , public std::enable_shared_from_this<ServerBase>
{
public:
    using Port = asio::ip::port_type;

    ServerBase(std::shared_ptr<asio::io_context> io_context,
               std::shared_ptr<asio::ssl::context> ssl_context, 
               const Port port);
    ~ServerBase() = default;

    void Start() override;
    void Stop() override;
    void Restart() override;
    bool IsStarted() const noexcept;

protected:
    virtual std::shared_ptr<SessionBase> CreateSession();
    virtual void HandleError(const asio::error_code& error);
    bool Multicast(const std::string_view data) override;
    void Accept() override;
    void OnStarted() override;
    void OnStopped() override;
    void OnRestarted() override;
    void OnAccepted() override; 
    void DisconnectAll();

    std::shared_ptr<SessionRegisterBase> m_session_register;
    std::shared_ptr<asio::io_context> m_io_context;
    std::shared_ptr<asio::ssl::context> m_ssl_context;
private:
    std::atomic<bool> m_started;
    asio::ip::tcp::endpoint m_endpoint;
    asio::ip::tcp::acceptor m_acceptor;
};

}

}