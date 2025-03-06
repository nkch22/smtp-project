#pragma once
/**
 * @file ISession.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-03-04
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include <string_view>

namespace SMTP
{

class ISession
{
public:
    virtual ~ISession() = default;

    /**
     * @brief 
     * 
     */
    virtual void Connect() = 0;

    /**
     * @brief 
     * 
     */
    virtual void Disconnect() = 0;

    /**
     * @brief 
     * 
     */
    virtual void Receive() = 0;

    /**
     * @brief 
     * 
     * @param data 
     * @return true 
     * @return false 
     */
    virtual bool Send(const std::string_view data) = 0;

    /**
     * @brief 
     * 
     */
    virtual void OnConnected() = 0;

    /**
     * @brief 
     * 
     */
    virtual void OnDisconnected() = 0;

    /**
     * @brief 
     * 
     * @param data 
     */
    virtual void OnReceived(const std::string_view data) = 0;

    /**
     * @brief 
     * 
     * @param sent 
     */
    virtual void OnSent(const std::size_t sent) = 0;
};

}