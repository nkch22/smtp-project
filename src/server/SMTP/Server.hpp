#pragma once
/**
 * @file Server.hpp
 * @author Oleksandr
 * @brief 
 * @version 0.1
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <functional>

#include "Session.hpp"
#include "SSL/ServerBase.hpp"
#include "Protocol/Parser.hpp"

namespace SMTP
{

/**
 * @brief SMTP server class implementation
 * 
 */
class Server : public SSL::ServerBase
{
public:
    using Port = asio::ip::port_type;

    /**
     * @brief Construct a new Server object
     * 
     * @param io_context 
     * @param ssl_context 
     * @param smtp_options 
     * @param port 
     */
    Server(std::shared_ptr<asio::io_context> io_context, 
           std::shared_ptr<asio::ssl::context> ssl_context, 
           std::function<std::shared_ptr<Context>()> context_generator,
           const Port port);
    ~Server() = default;
protected:

    /**
     * @brief Create a Session object
     * 
     * @return std::shared_ptr<SSL::SessionBase> 
     */
    std::shared_ptr<SSL::SessionBase> CreateSession() override;

    /**
     * @brief Function that is called after sessions is accepted
     * 
     */
    void OnAccepted() override;

    /**
     * @brief Function that is called after server is started
     * 
     */
    void OnStarted() override;

    std::function<std::shared_ptr<Context>()> m_context_generator;
};

}