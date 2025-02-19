#pragma once

#include "Command.hpp"

namespace SMTP
{
    namespace Protocol
    {
        class NoopCommand : public Command
        {
        public:
            constexpr static const char* COMMAND{"NOOP"};

            NoopCommand() = default;
            ~NoopCommand() = default;
            Response CreateResponse(const SessionContext& context) override;
        };
    }
}