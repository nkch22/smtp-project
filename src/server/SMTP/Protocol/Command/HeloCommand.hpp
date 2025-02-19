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
            constexpr static const char* COMMAND{"HELO"};

            HeloCommand() = default;
            ~HeloCommand() = default;
            Response CreateResponse(const SessionContext& context) override;

        private:
            std::string CreateMessage(const SessionContext& context) const;
        }; 
    }
}