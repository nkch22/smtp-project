#include "Parser.hpp"

namespace SMTP
{
    namespace Protocol
    {
        Parser::OptionalCommand Parser::TryParseRequest(const std::string& request)
        {
            return std::nullopt;
        }
    }
}