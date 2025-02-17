#pragma once

#include "Command.hpp"

namespace SMTP
{
    namespace Protocol
    {
        class NoopCommand : public Command
        {
        public:
            Response CreateResponse(const SessionContext& context) override;
        private:
        };
    }
}