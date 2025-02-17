#pragma once

#include <optional>
#include <memory>

#include "Command.hpp"
#include "SessionContext.hpp"

namespace SMTP
{
    namespace Protocol
    {
        class Parser
        {
        public:
            using OptionalCommand = std::optional<std::unique_ptr<Command>>;

            OptionalCommand TryParseRequest(const std::string& request, const SessionContext& context) const;
        private:
            OptionalCommand TryParseHelo(const std::string& request) const;
            OptionalCommand TryParseEhlo(const std::string& request, const SessionContext& context) const;
            OptionalCommand TryParseQuit(const std::string& request) const;
            std::string ToUpper(const std::string& string) const;
        };
    }
}