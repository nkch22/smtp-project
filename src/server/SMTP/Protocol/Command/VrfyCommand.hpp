#pragma once

#include "Command.hpp"

namespace SMTP
{
    namespace Protocol
    {
        class VrfyCommand : public Command
        {
        public:
            constexpr static const char* COMMAND{"VRFY"};
            VrfyCommand() = default;
            ~VrfyCommand() = default;
            Response CreateResponse(const SessionContext& context) override;
        };
    }
}