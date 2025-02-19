#pragma once

#include "Command.hpp"

namespace SMTP
{
    namespace Protocol
    {
        class RsetCommand : public Command
        {
        public:
            constexpr static const char* COMMAND{"RSET"};

            RsetCommand() = default;
            ~RsetCommand() = default;
            Response CreateResponse(const SessionContext& context) override;
        };
    }
}