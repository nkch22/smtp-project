#include "SSLSession.hpp"

#include <print>
#include <cassert>
#include <iostream>

namespace SMTP
{
    SSLSession::SSLSession(asio::ip::tcp::socket socket, std::shared_ptr<asio::ssl::context> ssl_context)
        : m_connected{false}
        , m_receiving{false}
        , m_handshaked{false}
        , m_sending{false}
        , m_stream{std::move(socket), *ssl_context}
        , m_receive_buffer{}
        , m_send_buffer{}
    {
    }

    asio::ssl::stream<asio::ip::tcp::socket>::next_layer_type& SSLSession::get_socket() noexcept
    {
        return m_stream.next_layer();
    }

    void SSLSession::Connect()
    {
        m_connected = true;
        OnConnected();
        auto self{shared_from_this()};
        auto async_handshake_handler{[this, self](const asio::error_code error)
        {
            if(IsHandshaked())
            {
                return;
            }

            if(!error)
            {
                m_handshaked = true;
                TryReceive();
                OnHandshaked();               
            }
            else
            {
                std::println("Error: {}", error.message());
                Disconnect();
            }
        }};
        m_stream.async_handshake(asio::ssl::stream_base::server, async_handshake_handler);
    }

    void SSLSession::Disconnect()
    {
        get_socket().close();
        m_handshaked = false;
        m_connected = false;
        OnDisconnected();
    }

    bool SSLSession::IsConnected() const noexcept
    {
        return m_connected;
    }

    bool SSLSession::IsHandshaked() const noexcept
    {
        return m_handshaked;
    }

    bool SSLSession::Send(std::string data)
    {
        if(!IsHandshaked())
        {
            return false;
        }
        if(std::size(data) == 0)
        {
            return true;
        }
        {
            std::scoped_lock lock{m_send_mutex};
            std::ostream out{&m_send_buffer};
            out << data;
        }
        TrySend();
        return true;
    }

    void SSLSession::Receive()
    {
        TryReceive();
    }

    void SSLSession::OnConnected()
    {
    }

    void SSLSession::OnDisconnected()
    {
    }

    void SSLSession::OnHandshaked()
    {
    }

    void SSLSession::OnReceived()
    {
    }

    void SSLSession::OnSent()
    {
    }

    void SSLSession::TryReceive()
    {
        if(m_receiving)
        {
            return;
        }
        if(!IsHandshaked())
        {
            return;
        }

        m_receiving = true;
        auto self{shared_from_this()};
        auto async_receive_handler{[this, self](const std::error_code error, const std::size_t size)
        {
            m_receiving = false;
            if(size > 0)
            {
                OnReceived();
            }
            if(!error)
            {
                TryReceive();
            }
            else
            {
                std::println("Error: {}", error.message());
                Disconnect();
            }
        }};

        asio::async_read(m_stream, m_receive_buffer, async_receive_handler);
    }

    void SSLSession::TrySend()
    {
        if(m_sending)
        {
            return;
        }
        if(!IsHandshaked())
        {
            return;
        }
        m_sending = true;
        auto self{shared_from_this()};
        auto async_write_handler{[this, self](asio::error_code error, const std::size_t size)
        {
            m_sending = false;
            if(!IsHandshaked())
            {
                return;
            }
            if(size > 0)
            {
                OnSent();
            }
            if(!error)
            {
                TrySend();
            }
            else
            {
                std::println("Error: {}", error.message());
                Disconnect();
            }
        }};
        asio::async_write(m_stream, m_send_buffer, async_write_handler);
    }
}