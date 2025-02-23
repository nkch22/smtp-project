#pragma once

#include <memory>
#include <atomic>

#include <asio.hpp>
#include <asio/ssl.hpp>

#include "../ISession.hpp"

namespace SMTP
{

namespace SSL
{

class Session 
    : public ISession
    , public std::enable_shared_from_this<Session>
{
public:
    Session(std::shared_ptr<asio::io_context> io_context, 
            std::shared_ptr<asio::ssl::context> ssl_context);
    ~Session() = default;

    void Connect() override;
    void Disconnect() override;
    void Receive() override;
    bool Send(const std::string_view data) override;

    bool IsConnected() const override;
    bool IsHandshaked() const;

    asio::ssl::stream<asio::ip::tcp::socket>& get_stream() noexcept;
    asio::ssl::stream<asio::ip::tcp::socket>::next_layer_type& get_socket() noexcept;
protected:
    virtual void OnHandshaked();
    void OnConnected() override;
    void OnDisconnected() override;
    void OnReceived(const std::string_view data) override;
    void OnSent(const std::size_t sent) override;

private:
    void TryReceive();
    void TrySend();
    void HandleError(const asio::error_code& error);
    void ClearBuffers();
    std::shared_ptr<asio::io_context> m_io_context;
    std::shared_ptr<asio::ssl::context> m_ssl_context;
    std::atomic<bool> m_connected;
    std::atomic<bool> m_handshaked;
    bool m_receiving;
    bool m_sending;
    asio::ssl::stream<asio::ip::tcp::socket> m_stream;
    std::mutex m_send_mutex;
    asio::streambuf m_receive_buffer;
    asio::streambuf m_send_buffer;
};

}

}