#pragma once

#include "Command.hpp"

namespace SMTP
{
    namespace Protocol
    {
        class EhloCommand : public Command
        {
        public:
            EhloCommand(const std::string& domain_or_address);
            ~EhloCommand(){}
            Response CreateResponse(const ServerContext& context) override;
        private:
            std::string CreateMessage(const ServerContext& context) const;

            std::string m_domain_of_address;
        };
    }
}