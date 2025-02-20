#pragma once

#include "ICommand.hpp"

namespace SMTP
{
    namespace Protocol
    {
        class ServiceReadyCommand : public ICommand
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