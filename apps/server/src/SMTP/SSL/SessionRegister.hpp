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
class SessionRegister
{
public:
    SessionRegister() = default;
    ~SessionRegister();

    /**
     * @brief Function that adds session to map
     * 
     * @param session 
     */
    void RegisterSession(std::shared_ptr<ISession> session);

    /**
     * @brief Function that removes element from map
     * 
     * @param session 
     */
    void UnregisterSession(std::shared_ptr<ISession> session);

    /**
     * @brief Function that sends data to all connected clients
     * 
     * @param data 
     * @return true 
     * @return false 
     */
    bool Multicast(std::string_view data);

    /**
     * @brief Function that disconnects all sessions
     * 
     */
    void DisconnectAll();

    /**
     * @brief Function that clears map
     * 
     */
    void Clear();

    std::shared_mutex& get_sessions_mutex();
private:
    std::shared_mutex m_sessions_mutex;
    std::unordered_set<std::shared_ptr<ISession>> m_sessions;
};

}

}