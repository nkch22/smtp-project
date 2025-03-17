#include "VrfyCommand.hpp"

namespace SMTP
{

namespace Protocol
{

Response VrfyCommand::CreateResponse(Context& options)
{
    const Response response{ReplyCode::Ok};
    return response;
}

OptionalCommand VrfyCommand::TryParseCommand(const std::string& request, const Context& options)
{
    if(request.contains(COMMAND))
    {
        return std::make_unique<VrfyCommand>();
    }
    return std::nullopt;
}

}

}