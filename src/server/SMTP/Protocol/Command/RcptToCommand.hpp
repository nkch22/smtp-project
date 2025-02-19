#pragma once

#include "Command.hpp"

namespace SMTP
{
    namespace Protocol
    {
        class RcptToCommand : public Command
        {
        public:
            constexpr static const char* COMMAND{"RCPT TO"};

            RcptToCommand() = default;
            ~RcptToCommand() = default;
            Response CreateResponse(const SessionContext& context) override;
        };
    }
}