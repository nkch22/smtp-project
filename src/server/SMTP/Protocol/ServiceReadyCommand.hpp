#pragma once

#include "Command.hpp"
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
            Response CreateResponse(const ServerContext& context) override;

        private:
            std::string ServiceReadyMessage(const ServerContext& context) const;
        };
    }
}