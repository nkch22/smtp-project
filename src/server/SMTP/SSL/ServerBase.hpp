#pragma once
/**
 * @file ServerBase.hpp
 * @author Oleksandr
 * @brief 
 * @version 0.1
 * @date 2025-03-04
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <memory>
#include <unordered_set>
#include <shared_mutex>

#include <asio.hpp>
#include <asio/ssl.hpp>

#include "../IServer.hpp"
#include "SessionBase.hpp"
#include "SessionRegister.hpp"

namespace SMTP
{

namespace SSL
{

/**
 * @brief Reusable Server Base class object
 * 
 */
class ServerBase 
    : public IServer
    , public std::enable_shared_from_this<ServerBase>
{
public:
    using Port = asio::ip::port_type;

    /**
     * @brief Construct a new Server Base object
     * 
     * @param io_context 
     * @param ssl_context 
     * @param port receives port number typically 465, 587
     */
    ServerBase(std::shared_ptr<asio::io_context> io_context,
               std::shared_ptr<asio::ssl::context> ssl_context, 
               const Port port);
    ~ServerBase() = default;

    /**
     * @brief Function that starts server asynchronously
     * 
     */
    void Start() override;

    /**
     * @brief Function that stops server asynchronously and Disconnect all sessions
     * 
     */
    void Stop() override;

    /**
     * @brief Function that restarts server if server is on
     * 
     */
    void Restart() override;
        
    /**
     * @brief Function that returns value of m_started
     * 
     * @return true if server is working
     * @return false if server is turned off
     */
    bool IsStarted() const noexcept;

protected:

    /**
     * @brief Create a Session object
     * 
     * @return std::shared_ptr<SessionBase> 
     */
    virtual std::shared_ptr<SessionBase> CreateSession();

    /**
     * @brief Function that handles errors
     * 
     * @param error 
     */
    virtual void HandleError(const asio::error_code& error);
    
    /**
     * @brief Function that sends data to all Sessions
     * 
     * @param data 
     * @return true 
     * @return false 
     */
    bool Multicast(const std::string_view data) override;

    /**
     * @brief Function that accepts and creates new Session
     * 
     */
    void Accept() override;

    /**
     * @brief Function that is called after server starts
     * 
     */
    void OnStarted() override;

    /**
     * @brief Function that is called after server stops
     * 
     */
    void OnStopped() override;

    /**
     * @brief Function called when server restarts
     * 
     */
    void OnRestarted() override;

    /**
     * @brief Function called when server accepts session
     * 
     */
    void OnAccepted() override; 
    
    /**
     * @brief Function that disconnects all sessions
     * 
     */
    void DisconnectAll();

    std::shared_ptr<SessionRegister> m_session_register;
    std::shared_ptr<asio::io_context> m_io_context;
    std::shared_ptr<asio::ssl::context> m_ssl_context;
private:
    std::atomic<bool> m_started;
    asio::ip::tcp::endpoint m_endpoint;
    asio::ip::tcp::acceptor m_acceptor;
};

}

}