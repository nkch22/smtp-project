#include "Parser.hpp"

#include <algorithm>

#include "HeloCommand.hpp"
#include "EhloCommand.hpp"
#include "QuitCommand.hpp"

namespace SMTP
{
    namespace Protocol
    {
        Parser::OptionalCommand Parser::TryParseRequest(const std::string& request) const
        {
            const auto upper_request{ToUpper(request)};
            if(auto helo{TryParseHelo(upper_request)}; 
               helo.has_value())
            {
                return helo;
            }
            else if(auto ehlo{TryParseEhlo(upper_request)}; 
                    ehlo.has_value())
            {
                return ehlo;
            }
            else if(auto quit{TryParseQuit(upper_request)};
                    quit.has_value())
            {
                return quit;
            }
            return std::nullopt;
        }

        Parser::OptionalCommand Parser::TryParseHelo(const std::string& request) const
        {
            if(request.contains(HeloCommand::COMMAND))
            {
                return std::make_unique<HeloCommand>();
            }
            return std::nullopt;
        }

        Parser::OptionalCommand Parser::TryParseEhlo(const std::string& request) const
        {
            if(request.contains(EhloCommand::COMMAND))
            {
            }
            return std::nullopt;
        }

        Parser::OptionalCommand Parser::TryParseQuit(const std::string& request) const
        {
            if(request.contains(QuitCommand::COMMAND))
            {
                return std::make_unique<QuitCommand>();
            }
            return std::nullopt;
        }

        std::string Parser::ToUpper(const std::string& string) const
        {
            std::string upper_copy{};
            upper_copy.resize(std::size(string));
            std::transform(std::begin(string), std::end(string), std::begin(upper_copy), ::toupper);
            return upper_copy;
        }
    }
}