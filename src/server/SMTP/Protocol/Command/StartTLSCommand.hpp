#pragma once

#include "ICommand.hpp"

namespace SMTP
{
    namespace Protocol
    {
        class StartTLSCommand : public ICommand
        {
        public:
            constexpr static const char* COMMAND{"STARTTLS"};

            StartTLSCommand() = default;
            ~StartTLSCommand() = default;
            Response CreateResponse(const SessionContext& context) override;
        };
    }
}