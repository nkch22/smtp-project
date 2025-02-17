#pragma once

#include "Command.hpp"

namespace SMTP
{
    namespace Protocol
    {
        class QuitCommand : public Command
        {
        public:
            ~QuitCommand(){}
            Response CreateResponse(const ServerContext& context) override;
        private:
            std::string CreateMessage(const ServerContext& context) const;
        };
    }
}