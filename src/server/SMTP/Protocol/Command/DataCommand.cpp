#include "DataCommand.hpp"

namespace SMTP
{

namespace Protocol
{

Response DataCommand::CreateResponse(const Options& options)
{
    const Response response{ReplyCode::Ok};
    return response;
}

OptionalCommand DataCommand::TryParseCommand(const std::string& request, const Options& options)
{
    return std::nullopt;
}

}

}