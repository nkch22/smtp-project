#include "NoopCommand.hpp"

namespace SMTP
{

namespace Protocol
{

Response NoopCommand::CreateResponse(const Options& options)
{
    const Response response{ReplyCode::Ok};
    return response;
}

OptionalCommand NoopCommand::TryParseCommand(const std::string& request, const Options& options)
{
    if(request.contains(COMMAND))
    {
        return std::make_unique<NoopCommand>();
    }
    return std::nullopt;
}

}

}