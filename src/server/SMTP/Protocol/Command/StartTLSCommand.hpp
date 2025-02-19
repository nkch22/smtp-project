#pragma once

#include "Command.hpp"

namespace SMTP
{
    namespace Protocol
    {
        class StartTLSCommand : public Command
        {
        public:
            constexpr static const char* COMMAND{"STARTTLS"};

            StartTLSCommand() = default;
            ~StartTLSCommand() = default;
            Response CreateResponse(const SessionContext& context) override;
        };
    }
}