#pragma once

#include <optional>
#include <memory>

#include "ICommand.hpp"

namespace SMTP
{
    namespace Protocol
    {
        class Parser
        {
        public:
            using OptionalCommand = std::optional<std::unique_ptr<ICommand>>;

            OptionalCommand TryParseRequest(const std::string& request) const;
        private:
            OptionalCommand TryParseHelo(const std::string& request) const;
            OptionalCommand TryParseEhlo(const std::string& request) const;
            OptionalCommand TryParseQuit(const std::string& request) const;
            std::string ToUpper(const std::string& string) const;
        };
    }
}