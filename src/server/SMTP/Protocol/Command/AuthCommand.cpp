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
    if(request.contains(COMMAND))
    {
        return std::make_unique<AuthCommand>();
    }
    return std::nullopt;
}

}

}