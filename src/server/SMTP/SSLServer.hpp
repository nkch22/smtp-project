#pragma once

#include <memory>
#include <list>

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
    private:
        void Accept();
        std::shared_ptr<SSLSession> CreateSession();

        bool m_started;
        std::shared_ptr<asio::io_context> m_io_context;
        std::shared_ptr<asio::ssl::context> m_ssl_context;
        asio::ip::tcp::acceptor m_acceptor;
        std::list<SSLSession> m_sessions;
    };
}