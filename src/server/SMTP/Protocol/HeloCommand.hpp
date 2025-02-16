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
            Response CreateResponse() override;

        private:
            std::string m_domain_of_address;
        }; 
    }
}