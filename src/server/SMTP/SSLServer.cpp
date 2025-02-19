#include "SSLServer.hpp"

namespace SMTP
{
    SSLServer::SSLServer(std::shared_ptr<asio::io_context> io_context, std::shared_ptr<asio::ssl::context> ssl_context)
        : m_started{false}
        , m_io_context{io_context}
        , m_acceptor{*io_context}
        , m_ssl_context{ssl_context}
    {
    }

    bool SSLServer::IsStarted() const noexcept
    {
        return m_started;
    }

    void SSLServer::Listen(const Port port)
    {
        const asio::ip::tcp::endpoint endpoint{asio::ip::tcp::v4(), port};
        m_acceptor.open(endpoint.protocol());
        m_acceptor.set_option(asio::ip::tcp::acceptor::reuse_address{true});
        m_acceptor.bind(endpoint);
        m_acceptor.listen();
        Accept();
    }

    void SSLServer::Start()
    {
        if(IsStarted())
        {
            return;
        }

        m_started = true;
        m_io_context->run();
    }

    void SSLServer::Stop()
    {
    }

    std::shared_ptr<SSLSession> SSLServer::CreateSession()
    {
        return std::make_shared<SSLSession>();
    }

    void SSLServer::Accept()
    {
        auto async_accept_handler{[this](const asio::error_code error)
        {
            if(!error)
            {
                CreateSession();
            }
        }};
    }
}