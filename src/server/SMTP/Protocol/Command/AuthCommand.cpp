#include "AuthCommand.hpp"

namespace SMTP
{

namespace Protocol
{

Response AuthCommand::CreateResponse(const Options& options)
{
    const Response response{ReplyCode::Ok};
    return response;
}

OptionalCommand AuthCommand::TryParseCommand(const std::string& request, const Options& options)
{
    return std::nullopt;
}

}

}