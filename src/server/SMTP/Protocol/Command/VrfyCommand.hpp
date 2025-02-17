#pragma once

#include "Command.hpp"

namespace SMTP
{
    namespace Protocol
    {
        class VrfyCommand : public Command
        {
        public:
            Response CreateResponse(const SessionContext& context) override;
        };
    }
}