#pragma once

#include "Command.hpp"

namespace SMTP
{
    namespace Protocol
    {
        class AuthCommand : public Command
        {
        public:
            constexpr static const char* COMMAND{"DATA"};

            AuthCommand() = default;
            ~AuthCommand() = default;
            Response CreateResponse(const SessionContext& context) override;
        };
    }
}