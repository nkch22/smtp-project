#pragma once

#include "ICommand.hpp"

namespace SMTP
{
    namespace Protocol
    {
        class EhloCommand : public ICommand
        {
        public:
            constexpr static const char* COMMAND{"EHLO"};

            EhloCommand(const std::string& domain_or_address);
            ~EhloCommand() = default;
            Response CreateResponse(const SessionContext& context) override;
        private:
            std::string CreateMessage(const SessionContext& context) const;

            std::string m_domain_of_address;
        };
    }
}