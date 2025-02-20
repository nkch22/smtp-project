#pragma once

#include <string_view>

namespace SMTP
{
    class ISessionHandler
    {
    public:
        virtual ~ISessionHandler() = default;

        virtual void OnConnected() = 0;
        virtual void OnDisconnected() = 0;
        virtual void OnReceived(const std::string_view data) = 0;
        virtual void OnSent() = 0;
    };
}