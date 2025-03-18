#include "RsetCommand.hpp"

namespace SMTP
{

namespace Protocol
{

Response RsetCommand::CreateResponse(Context& options)
{
    const Response response{ReplyCode::Ok};
    return response;
}

OptionalCommand RsetCommand::TryParseCommand(const std::string& request, const Context& options)
{
    if(request.contains(COMMAND))
    {
        return std::make_unique<RsetCommand>();
    }
    return std::nullopt;
}

}

}