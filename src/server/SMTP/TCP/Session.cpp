#include "Session.hpp"

#include <print>

namespace SMTP
{

namespace TCP
{

Session::Session(std::shared_ptr<asio::io_context> io_context, asio::ip::tcp::socket socket)
    : m_io_context{io_context}
    , m_connected{false}
    , m_receiving{false}
    , m_sending{false}
    , m_socket{std::move(socket)}
    , m_receive_buffer{}
{
}

void Session::Connect()
{
    if(IsConnected())
    {
        return;
    }
    m_connected = true;
    TryReceive();
    OnConnected();
}

void Session::Disconnect()
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
        m_socket.close();
        m_connected = false;
        m_receiving = false;
        m_sending = false;
        ClearBuffers();
        OnDisconnected();
    }};
    m_io_context->dispatch(disconnect_handler);
}

void Session::Receive()
{
    TryReceive();
}

bool Session::Send(const std::string_view data)
{
    if(!IsConnected())
    {
        return false;
    }
    if(std::size(data) == 0)
    {
        return true;
    }
    {
        std::scoped_lock lock{m_send_mutex};
        std::copy(std::begin(data), std::end(data), 
            std::ostreambuf_iterator<char>{&m_send_buffer});
    }
    auto self{shared_from_this()};
    auto send_handler{[this, self]
    {
        TrySend();
    }};
    m_io_context->dispatch(send_handler);
    return true;
}

bool Session::IsConnected() const noexcept
{
    return m_connected;
}

void Session::TryReceive()
{
    if(m_receiving)
    {
        return;
    }
    if(!IsConnected())
    {
        return;
    }

    m_receiving = true;
    auto self{shared_from_this()};
    auto async_receive_handler{[this, self](const asio::error_code& error, const std::size_t size)
    {
        m_receiving = false;
        if(!IsConnected())
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
    asio::async_read_until(m_socket, m_receive_buffer, "\r\n", async_receive_handler);
}

void Session::TrySend()
{
    if(m_sending)
    {
        return;
    }
    if(!IsConnected())
    {
        return;
    }
    m_sending = true;
    auto self{shared_from_this()};
    auto async_write_receiver{[this, self](const asio::error_code& error, const std::size_t size)
    {
        m_sending = false;
        
        if(!IsConnected())
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
    asio::async_write(m_socket, m_send_buffer, async_write_receiver);
}

void Session::HandleError(const asio::error_code& error)
{
    std::println("Error: {}", error.message());
}

void Session::ClearBuffers()
{
    std::scoped_lock lock{m_send_mutex};
    m_send_buffer.consume(std::size(m_send_buffer));
    m_receive_buffer.consume(std::size(m_receive_buffer));
}

asio::ip::tcp::socket& Session::get_socket() noexcept
{
    return m_socket;
}

void Session::OnConnected()
{
}

void Session::OnDisconnected()
{
}

void Session::OnReceived(const std::string_view data)
{
}

void Session::OnSent(const std::size_t sent)
{
}

}

}