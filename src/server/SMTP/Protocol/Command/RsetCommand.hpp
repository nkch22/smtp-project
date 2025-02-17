#pragma once

#include "Command.hpp"

namespace SMTP
{
    namespace Protocol
    {
        class RsetCommand : public Command
        {
        public:
            Response CreateResponse(const SessionContext& context) override;
        private:
        };
    }
}