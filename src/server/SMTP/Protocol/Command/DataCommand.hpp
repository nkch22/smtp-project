#pragma once

#include "Command.hpp"

namespace SMTP
{
    namespace Protocol
    {
        class DataCommand : public Command
        {
        public:
            constexpr static const char* COMMAND{"DATA"};

            DataCommand() = default;
            ~DataCommand() = default;
            Response CreateResponse(const SessionContext& context) override;
        };
    }
}