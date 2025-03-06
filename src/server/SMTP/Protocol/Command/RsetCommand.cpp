#include "RsetCommand.hpp"

namespace SMTP
{

namespace Protocol
{

Response RsetCommand::CreateResponse(const Options& options)
{
    const Response response{ReplyCode::Ok};
    return response;
}

OptionalCommand RsetCommand::TryParseCommand(const std::string& request, const Options& options)
{
    if(request.contains(COMMAND))
    {
        return std::make_unique<RsetCommand>();
    }
    return std::nullopt;
}

}

}