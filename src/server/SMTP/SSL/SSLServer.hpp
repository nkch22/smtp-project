#pragma once

#include <memory>
#include <unordered_set>

#include <asio.hpp>
#include <asio/ssl.hpp>

#include "SSLSession.hpp"

namespace SMTP
{
    class SSLServer
    {
    public:
        using Port = asio::ip::port_type;

        SSLServer(std::shared_ptr<asio::io_context> io_context, std::shared_ptr<asio::ssl::context> ssl_context);
        virtual ~SSLServer() = default;
        
        bool IsStarted() const noexcept;

        virtual void Listen(const Port port);
        virtual void Start();
        virtual void Stop();
        virtual std::shared_ptr<SSLSession> CreateSession(asio::ip::tcp::socket socket);

    private:
        void Accept();

        bool m_started;

        std::shared_ptr<asio::io_context> m_io_context;
        std::shared_ptr<asio::ssl::context> m_ssl_context;
        asio::ip::tcp::acceptor m_acceptor;
        std::unordered_set<std::shared_ptr<SSLSession>> m_sessions;
    };
}