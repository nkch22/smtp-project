#include "SessionBase.hpp"

namespace SMTP
{

namespace SSL
{

SessionBase::SessionBase(std::shared_ptr<asio::io_context> io_context, 
                         std::shared_ptr<asio::ssl::context> ssl_context)
    : m_io_context{io_context}
    , m_ssl_context{ssl_context}
    , m_connected{false}
    , m_handshaked{false}
    , m_receiving{false}
    , m_sending{false}
    , m_stream{*m_io_context, *m_ssl_context}
    , m_receive_buffer{}
    , m_send_buffer{}
    , m_send_mutex{}
{
}

void SessionBase::Connect()
{
    m_connected = true;

    OnConnected();

    auto self{shared_from_this()};
    auto async_handshake_handler{[this, self](const asio::error_code& error)
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
            HandleError(error);
            Disconnect();
        }

    }};
    m_stream.async_handshake(asio::ssl::stream_base::server, async_handshake_handler);
}

void SessionBase::Disconnect()
{
    if(!IsConnected())
    {
        return;
    }

    auto self{shared_from_this()};
    auto disconnect_handler{[this, self]
    {
        if(!IsConnected())
        {
            return;
        }
        asio::error_code error{};
        get_socket().cancel(error);
        HandleError(error);
        auto async_shutdown_handler{[this, self](const std::error_code& error)
        {
            if(!IsConnected())
            {
                return;
            }
            HandleError(error);
            get_socket().close();
            m_handshaked = false;
            m_connected = false;
            m_receiving = false;
            m_sending = false;
            ClearBuffers();
            OnDisconnected();
        }};
        m_stream.async_shutdown(async_shutdown_handler);
    }};

    m_io_context->dispatch(disconnect_handler);
}

void SessionBase::Receive()
{
    TryReceive();
}

bool SessionBase::Send(const std::string_view data)
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
        /*std::copy(std::begin(data), std::end(data), 
            std::ostreambuf_iterator<char>{&m_send_buffer});*/
    }

    auto self{shared_from_this()};
    auto send_handler{[this, self]
    {
        TrySend();
    }};
    m_io_context->dispatch(send_handler);
    return true;
}

bool SessionBase::IsConnected() const noexcept
{
    return m_connected;
}

bool SessionBase::IsHandshaked() const noexcept
{
    return m_handshaked;
}

void SessionBase::ClearBuffers()
{
    std::scoped_lock lock{m_send_mutex};
}

asio::ssl::stream<asio::ip::tcp::socket>& SessionBase::get_stream() noexcept
{
    return m_stream;
}

asio::ssl::stream<asio::ip::tcp::socket>::next_layer_type& SessionBase::get_socket() noexcept
{
    return m_stream.next_layer();
}

void SessionBase::TryReceive()
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
    auto async_receive_handler{[this, self](const std::error_code& error, const std::size_t size)
    {
        m_receiving = false;
        
        if(!IsHandshaked())
        {
            return;
        }

        if(size > 0)
        {
            std::string data{std::istreambuf_iterator<char>{&m_receive_buffer},
            std::istreambuf_iterator<char>{}};
            OnReceived(data);
        }
        
        if(!error)
        {
            TryReceive();
        }
        else 
        {
            HandleError(error);
            Disconnect();
        }
    }};
    asio::async_read_until(m_stream, m_receive_buffer, "\r\n", async_receive_handler);
}

void SessionBase::TrySend()
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
    auto async_write_handler{[this, self](const asio::error_code& error, const std::size_t size)
        {
            m_sending = false;

            if(!IsHandshaked())
            {
                return;
            }

        if(size > 0)
        {
            OnSent(size);
        }
        if(!error)
        {
            TrySend();
        }
        else
        {
            HandleError(error);
            Disconnect();
        }
    }};
    asio::async_write(m_stream, m_send_buffer, async_write_handler);
}

void SessionBase::HandleError(const asio::error_code& error)
{
}

void SessionBase::OnConnected()
{
}

void SessionBase::OnHandshaked()
{
}

void SessionBase::OnDisconnected()
{
}

void SessionBase::OnReceived(const std::string_view data)
{
}

void SessionBase::OnSent(const std::size_t sent)
{
}

}

}