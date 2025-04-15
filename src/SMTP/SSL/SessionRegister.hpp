#pragma once

#include <unordered_set>
#include <memory>
#include <shared_mutex>

#include "../ISession.hpp"

namespace SMTP
{

namespace SSL
{

class SessionRegister
{
public:
    SessionRegister() = default;
    ~SessionRegister();

    void RegisterSession(std::shared_ptr<ISession> session);
    void UnregisterSession(std::shared_ptr<ISession> session);
    bool Multicast(std::string_view data);
    void DisconnectAll();
    void Clear();

    std::shared_mutex& get_sessions_mutex();
private:
    std::shared_mutex m_sessions_mutex;
    std::unordered_set<std::shared_ptr<ISession>> m_sessions;
};

}

}