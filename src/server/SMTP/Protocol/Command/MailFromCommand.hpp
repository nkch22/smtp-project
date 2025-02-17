#pragma once

#include "Command.hpp"

namespace SMTP
{
    namespace Protocol
    {
        class MailFromCommand : public Command
        {
        public:
            Response CreateResponse(const SessionContext& context) override;
        private:
        };
    }
}