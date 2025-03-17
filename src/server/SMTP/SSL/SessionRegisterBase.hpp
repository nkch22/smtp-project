#pragma once
/**
 * @file SessionRegister.hpp
 * @author Oleksandr
 * @brief 
 * @version 0.1
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <unordered_set>
#include <memory>
#include <shared_mutex>

#include "../ISession.hpp"

namespace SMTP
{

namespace SSL
{

/**
 * @brief Class that handles all sessions
 * 
 */
class SessionRegisterBase
{
public:
    SessionRegisterBase() = default;
    ~SessionRegisterBase();

    /**
     * @brief Function that adds session to map
     * 
     * @param session 
     */
    virtual void RegisterSession(std::shared_ptr<ISession> session);

    /**
     * @brief Function that removes element from map
     * 
     * @param session 
     */
    virtual void UnregisterSession(std::shared_ptr<ISession> session);

    /**
     * @brief Function that sends data to all connected clients
     * 
     * @param data 
     * @return true 
     * @return false 
     */
    virtual bool Multicast(const std::string_view data);

    /**
     * @brief Function that disconnects all sessions
     * 
     */
    virtual void DisconnectAll();

    /**
     * @brief Function that clears map
     * 
     */
    virtual void Clear();

    std::shared_mutex& get_sessions_mutex();
private:
    std::shared_mutex m_sessions_mutex;
    std::unordered_set<std::shared_ptr<ISession>> m_sessions;
};

}

}