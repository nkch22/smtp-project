#pragma once

#include "Command.hpp"

namespace SMTP
{
    namespace Protocol
    {
        class MailFromCommand : public Command
        {
        public:
            constexpr static const char* COMMAND{"MAIL FROM"};

            MailFromCommand() = default;
            ~MailFromCommand() = default;
            Response CreateResponse(const SessionContext& context) override;
        };
    }
}