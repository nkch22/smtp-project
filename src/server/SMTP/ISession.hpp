#pragma once

#include <string_view>

namespace SMTP
{

class ISession
{
public:
    virtual ~ISession() = default;

    virtual void Connect() = 0;
    virtual void Disconnect() = 0;
    virtual void Receive() = 0;
    virtual bool Send(const std::string_view data) = 0;

    virtual bool IsConnected() const = 0;
protected:
    virtual void OnConnected() = 0;
    virtual void OnDisconnected() = 0;
    virtual void OnReceived(const std::string_view data) = 0;
    virtual void OnSent(const std::size_t sent) = 0;
};

}