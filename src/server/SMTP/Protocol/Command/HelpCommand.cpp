#include "HelpCommand.hpp"

namespace SMTP
{

namespace Protocol
{

Response HelpCommand::CreateResponse(const Options& options)
{
    const Response response{Protocol::ReplyCode::Ok};
    return response;
}

OptionalCommand HelpCommand::TryParseCommand(const std::string& request, const Options& options)
{
    return std::nullopt;
}

}

}