#pragma once

#include "Command.hpp"

namespace SMTP
{
    namespace Protocol
    {
        class ServiceReadyCommand : public Command
        {
        public:
            ServiceReadyCommand() = default;
            ~ServiceReadyCommand() = default;
            Response CreateResponse(const SessionContext& context) override;

        private:
            std::string ServiceReadyMessage(const SessionContext& context) const;
        };
    }
}