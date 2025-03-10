#include "VrfyCommand.hpp"

namespace SMTP
{

namespace Protocol
{

Response VrfyCommand::CreateResponse(const Options& options)
{
    const Response response{ReplyCode::Ok};
    return response;
}

OptionalCommand VrfyCommand::TryParseCommand(const std::string& request, const Options& options)
{
    if(request.contains(COMMAND))
    {
        return std::make_unique<VrfyCommand>();
    }
    return std::nullopt;
}

}

}