#pragma once

#include <string>

#include "Command.hpp"

namespace SMTP
{
    namespace Protocol
    {
        class HeloCommand : public Command
        {
        public:
            HeloCommand(){}
            ~HeloCommand(){}
            Response CreateResponse(const ServerContext& context) override;

        private:
            std::string CreateMessage(const ServerContext& context) const;
        }; 
    }
}