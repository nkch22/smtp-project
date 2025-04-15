#pragma once

#include <unordered_set>
#include <memory>
#include <shared_mutex>

#include "../ISession.hpp"

namespace SMTP
{

namespace SSL
{

class SessionRegisterBase
{
public:
    SessionRegisterBase() = default;
    ~SessionRegisterBase();

    virtual void RegisterSession(std::shared_ptr<ISession> session);
    virtual void UnregisterSession(std::shared_ptr<ISession> session);
    virtual bool Multicast(const std::string_view data);
    virtual void DisconnectAll();
    virtual void Clear();

    std::shared_mutex& get_sessions_mutex();
private:
    std::shared_mutex m_sessions_mutex;
    std::unordered_set<std::shared_ptr<ISession>> m_sessions;
};

}

}