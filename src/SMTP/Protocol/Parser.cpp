#include "Parser.hpp"

#include <sstream>

#include "ParserHelper.hpp"
#include "HeloCommand.hpp"
#include "EhloCommand.hpp"
#include "QuitCommand.hpp"
#include "DataCommand.hpp"
#include "AuthCommand.hpp"
#include "MailCommand.hpp"
#include "NoopCommand.hpp"
#include "RcptCommand.hpp"
#include "RsetCommand.hpp"
#include "VrfyCommand.hpp"
#include "HelpCommand.hpp"
#include "Utils.hpp"

namespace SMTP
{

namespace Protocol
{

OptionalCommand Parser::TryParseRequest(const std::string& request, const Context& options) const
{
    const auto upper_request{ToUpper(request)};
    return ParserHelper<HeloCommand, EhloCommand, 
                        HelpCommand, QuitCommand, 
                        DataCommand, AuthCommand,
                        MailCommand, NoopCommand,
                        QuitCommand, RcptCommand,
                        RsetCommand, VrfyCommand>::TryParse(request, options);
}

}

}