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
    return std::nullopt;
}

}

}