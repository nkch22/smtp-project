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
            HeloCommand(const std::string& domain_or_address);
            ~HeloCommand(){}
            Response CreateResponse(const ServerContext& context) override;

        private:
            std::string CreateMessage(const ServerContext& context) const;

            std::string m_domain_of_address;
        }; 
    }
}