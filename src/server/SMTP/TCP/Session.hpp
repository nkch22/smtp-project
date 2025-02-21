#pragma once

#include <atomic>
#include <memory>
#include <mutex>

#include <asio.hpp>

#include "../ISession.hpp"

namespace SMTP
{

namespace TCP
{

class Session
    : public ISession
    , public std::enable_shared_from_this<Session>
{
public: 
    Session(std::shared_ptr<asio::io_context> io_context, asio::ip::tcp::socket socket);
    ~Session() = default;

    void Connect() override;
    void Disconnect() override;
    void Receive() override;
    bool Send(const std::string_view data) override;

    bool IsConnected() const override;

    void OnConnected() override;
    void OnDisconnected() override;
    void OnReceived(const std::string_view data) override;
    void OnSent(const std::size_t sent) override;
private:
    void TryReceive();
    void TrySend();
    void ClearBuffers();
    void HandleError(const asio::error_code& error);

    std::atomic<bool> m_connected;
    bool m_receiving;
    bool m_sending;
    std::shared_ptr<asio::io_context> m_io_context;
    asio::ip::tcp::socket m_socket;
    asio::streambuf m_receive_buffer;
    asio::streambuf m_send_buffer;
    std::mutex m_send_mutex;
};

}

}