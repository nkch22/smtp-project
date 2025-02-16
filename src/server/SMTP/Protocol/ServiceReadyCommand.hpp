#pragma once

#include "Command.hpp"
#include "CommandContext.hpp"
#include "Response.hpp"

namespace SMTP
{
    namespace Protocol
    {
        class ServiceReadyCommand : public Command
        {
        public:
            ServiceReadyCommand(){}
            ~ServiceReadyCommand(){}
            Response CreateResponse(const CommandContext& context) override;
        private:
            std::string ServiceReadyMessage(const CommandContext& context) const;
        };
    }
}