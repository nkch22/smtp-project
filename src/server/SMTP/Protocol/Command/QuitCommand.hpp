#pragma once

#include "Command.hpp"

namespace SMTP
{
    namespace Protocol
    {
        class QuitCommand : public Command
        {
        public:
            constexpr static const char* COMMAND{"QUIT"};

            QuitCommand() = default;
            ~QuitCommand() = default;
            Response CreateResponse(const SessionContext& context) override;
        private:
            std::string CreateMessage(const SessionContext& context) const;
        };
    }
}