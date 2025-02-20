#include "Session.hpp"

#include <print>

namespace SMTP
{
    namespace TCP
    {
        Session::Session(std::shared_ptr<asio::io_context> io_context)
            : m_connected{false}
            , m_receiving{false}
            , m_sending{false}
            , m_io_context{io_context}
            , m_socket{*io_context}
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
            auto disconnect_handler{[this, self]{
                if(!IsConnected())
                {
                    return;
                }
                m_socket.close();
                m_connected = false;
                m_receiving = false;
                m_sending = false;
                OnDisconnected();
            }};
            m_io_context->dispatch(disconnect_handler);
            return;
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
                //TODO: scoped_lock
            }
            auto self{shared_from_this()};
            auto send_handler{[this, self]
            {
                TrySend();
            }};
            m_io_context->dispatch(send_handler);
            return true;
        }

        bool Session::IsConnected() const
        {
            return m_connected;
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

        void Session::OnSent()
        {
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
                    /*TODO: READ DATA*/
                    OnReceived("");
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
            asio::async_read(m_socket, m_receive_buffer, async_receive_handler);
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
            if(/*empty*/false)
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
            asio::async_write(m_socket, m_receive_buffer, async_write_receiver);
        }
    }
}