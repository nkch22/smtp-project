#pragma once
/**
 * @file Session.hpp
 * @author Oleksandr
 * @brief 
 * @version 0.1
 * @date 2025-03-04
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "Protocol/Parser.hpp"
#include "SSL/SessionBase.hpp"
#include "Context.hpp"

namespace SMTP
{

class Session : public SSL::SessionBase
{
public:
    /**
     * @brief Construct a new Session object
     * 
     * @param io_context 
     * @param ssl_context 
     * @param smtp_parser 
     */
    Session(std::shared_ptr<asio::io_context> io_context, 
            std::shared_ptr<asio::ssl::context> ssl_context, 
            std::shared_ptr<SSL::SessionRegisterBase> session_register,
            std::shared_ptr<Context> context);
    ~Session() = default;
protected:
    /**
     * @brief Function that is called after session is connected with server
     * 
     */
    void OnConnected() override;

    /**
     * @brief Function that is called after session disconnects
     * 
     */
    void OnDisconnected() override;

    /**
     * @brief Function that is called after session receives data
     * 
     * @param data 
     */
    void OnReceived(const std::string_view data) override;

    /**
     * @brief Function that is called after data is sent
     * 
     * @param sent 
     */
    void OnSent(const std::size_t sent) override;

    /**
     * @brief Function that is called after handshake
     * 
     */
    void OnHandshaked() override;
    
    virtual void HandleResponse(const Protocol::Response& response);
private:    
    void HandleReplyCode(const Protocol::ReplyCode reply_code);

    Protocol::Parser m_smtp_parser;
    std::shared_ptr<Context> m_context;
};

}