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
    };
}