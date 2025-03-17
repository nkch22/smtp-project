#include "Parser.hpp"

#include <algorithm>
#include <sstream>

#include "ParserHelper.hpp"
#include "HeloCommand.hpp"
#include "EhloCommand.hpp"
#include "QuitCommand.hpp"
#include "DataCommand.hpp"
#include "AuthCommand.hpp"
#include "MailFromCommand.hpp"
#include "NoopCommand.hpp"
#include "RcptToCommand.hpp"
#include "RsetCommand.hpp"
#include "VrfyCommand.hpp"
#include "HelpCommand.hpp"

namespace SMTP
{

namespace Protocol
{

OptionalCommand Parser::TryParseRequest(const std::string& request, const Context& custom_options) const
{
    const auto upper_request{ToUpper(request)};
    return ParserHelper<HeloCommand, EhloCommand, 
                        HelpCommand, QuitCommand, 
                        DataCommand, AuthCommand,
                        MailFromCommand, NoopCommand,
                        QuitCommand, RcptToCommand,
                        RsetCommand, VrfyCommand>::TryParse(request, custom_options);
}

std::string Parser::ToUpper(const std::string& string)
{
    std::string upper_copy{};
    upper_copy.resize(std::size(string));
    std::transform(std::begin(string), std::end(string), std::begin(upper_copy), ::toupper);
    return upper_copy;
}

}

}