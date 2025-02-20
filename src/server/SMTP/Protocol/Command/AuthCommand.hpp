#pragma once

#include "ICommand.hpp"

namespace SMTP
{
    namespace Protocol
    {
        class AuthCommand : public ICommand
        {
        public:
            constexpr static const char* COMMAND{"DATA"};

            AuthCommand() = default;
            ~AuthCommand() = default;
            Response CreateResponse(const SessionContext& context) override;
        };
    }
}