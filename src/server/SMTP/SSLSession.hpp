#pragma once

#include <string>
#include <memory>
#include <mutex>
#include <atomic>

#include <asio.hpp>
#include <asio/ssl.hpp>

namespace SMTP
{
    class SSLSession : public std::enable_shared_from_this<SSLSession>
    {
    public:
        SSLSession(asio::ip::tcp::socket socket, std::shared_ptr<asio::ssl::context> ssl_context);
        virtual ~SSLSession() = default;

        asio::ssl::stream<asio::ip::tcp::socket>::next_layer_type& get_socket() noexcept;

        virtual void Connect();
        virtual void Disconnect();

        bool IsConnected() const noexcept;
        bool IsHandshaked() const noexcept;

        virtual void Receive();
        virtual bool Send(std::string data);

        virtual void OnConnected();
        virtual void OnDisconnected();
        virtual void OnHandshaked();
        virtual void OnReceived();
        virtual void OnSent();

        void TryReceive();
        void TrySend();
    private:
        bool m_sending;
        bool m_connected;
        bool m_handshaked;
        bool m_receiving;

        asio::ssl::stream<asio::ip::tcp::socket> m_stream;
        asio::streambuf m_receive_buffer;
        asio::streambuf m_send_buffer;
        std::mutex m_send_mutex;
    };
}