#pragma once
/**
 * @file SessionBase.hpp
 * @author Oleksandr
 * @brief 
 * @version 0.1
 * @date 2025-03-04
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <memory>
#include <optional>
#include <atomic>
#include <vector>

#include <asio.hpp>
#include <asio/ssl.hpp>

#include "../ISession.hpp"
#include "SessionRegisterBase.hpp"

namespace SMTP
{

namespace SSL
{

/**
 * @brief Reusable Session Base class object
 * 
 */
class SessionBase 
    : public ISession
    , public std::enable_shared_from_this<SessionBase>
{
public:
    /**
     * @brief Construct a new Session Base object
     * 
     * @param io_context 
     * @param ssl_context 
     */
    SessionBase(std::shared_ptr<asio::io_context> io_context, 
                std::shared_ptr<asio::ssl::context> ssl_context,
                std::shared_ptr<SessionRegisterBase> session_register,
                const std::size_t max_bytes_receive_size);
    ~SessionBase() = default;

    /**
     * @brief Function that Connects Session with Client
     * 
     */
    void Connect() override;

    /**
     * @brief Function that Disconnect Session with Client
     * 
     */
    void Disconnect() override;

    /**
     * @brief Function that receives data from clients
     * 
     */
    void Receive() override;

    /**
     * @brief Function that sends data to client
     * 
     * @param data 
     * @return true 
     * @return false 
     */
    bool Send(const std::string_view data) override;

    /**
     * @brief Function that returns value of m_connected
     * 
     * @return true session is connected to client
     * @return false session is not connected with client
     */
    bool IsConnected() const noexcept;

    /**
     * @brief Function that returns value of m_handshaked
     * 
     * @return true session did handshake with client
     * @return false session is not handshaked with client
     */
    bool IsHandshaked() const noexcept;

    /**
     * @brief Function that returns stream object
     * 
     * @return asio::ssl::stream<asio::ip::tcp::socket>& 
     */
    asio::ssl::stream<asio::ip::tcp::socket>& get_stream() noexcept;

    /**
     * @brief Function that return sockect object
     * 
     * @return asio::ssl::stream<asio::ip::tcp::socket>::next_layer_type& 
     */
    asio::ssl::stream<asio::ip::tcp::socket>::next_layer_type& get_socket() noexcept;
protected:
    /**
     * @brief Function that is called when client and server handshaked
     * 
     */
    virtual void OnHandshaked();

    /**
     * @brief Function that is called when error occurs
     * 
     * @param error 
     */
    virtual void HandleError(const asio::error_code& error);

    /**
     * @brief Function that is called when session connects to client
     * 
     */
    void OnConnected() override;

    /**
     * @brief Function that is called when session disconnects from client
     * 
     */
    void OnDisconnected() override;

    /**
     * @brief Function that is called when server receives data from client
     * 
     * @param data 
     */
    void OnReceived(const std::string_view data) override;

    /**
     * @brief Function that is called when data sent to client
     * 
     * @param sent 
     */
    void OnSent(const std::size_t sent) override;

    std::shared_ptr<SessionRegisterBase> m_session_register;
    std::shared_ptr<asio::io_context> m_io_context;
    std::shared_ptr<asio::ssl::context> m_ssl_context;

    asio::ssl::stream<asio::ip::tcp::socket> m_stream;
    std::mutex m_send_mutex;
private:

    /**
     * @brief Function that reads data from client
     * 
     */
    void TryReceive();

    /**
     * @brief Function that send data to client
     * 
     */
    void TrySend();

    /**
     * @brief Function that clears all buffers
     * 
     */
    void ClearBuffers();

    std::atomic<bool> m_connected;
    std::atomic<bool> m_handshaked;
    bool m_receiving;
    bool m_sending;
    std::vector<char> m_receive_buffer;
    asio::streambuf m_send_buffer;
};

}

}