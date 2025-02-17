#pragma once

#include <optional>
#include <memory>

#include "Command.hpp"

namespace SMTP
{
    namespace Protocol
    {
        class Parser
        {
        public:
            using OptionalCommand = std::optional<std::unique_ptr<Command>>;

            OptionalCommand TryParseRequest(const std::string& request);
        private:
        };
    }
}