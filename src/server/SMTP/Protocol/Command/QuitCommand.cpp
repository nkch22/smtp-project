#include "QuitCommand.hpp"

#include <format>

namespace SMTP
{

namespace Protocol
{

Response QuitCommand::CreateResponse(const Options& options)
{
    const Response response{ReplyCode::ServiceClosingTransmissionChannel, 
                            "Service closing transmission channel"};
    return response;
}

OptionalCommand QuitCommand::TryParseCommand(const std::string& request, const Options& options)
{
    return std::nullopt;
}

}

}