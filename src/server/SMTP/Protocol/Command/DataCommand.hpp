#pragma once

#include "Command.hpp"

namespace SMTP
{
    namespace Protocol
    {
        class DataCommand : public Command
        {
        public:
            Response CreateResponse(const SessionContext& context) override;
        private:
        };
    }
}