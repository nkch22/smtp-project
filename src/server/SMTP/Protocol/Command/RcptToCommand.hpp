#pragma once

#include "ICommand.hpp"

namespace SMTP
{
    namespace Protocol
    {
        class RcptToCommand : public ICommand
        {
        public:
            constexpr static const char* COMMAND{"RCPT TO"};

            RcptToCommand() = default;
            ~RcptToCommand() = default;
            Response CreateResponse(const SessionContext& context) override;
        };
    }
}