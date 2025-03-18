#include "QuitCommand.hpp"

#include <format>

namespace SMTP
{

namespace Protocol
{

Response QuitCommand::CreateResponse(Context& options)
{
    const Response response{ReplyCode::ServiceClosingTransmissionChannel, 
                            "Service closing transmission channel"};
    return response;
}

OptionalCommand QuitCommand::TryParseCommand(const std::string& request, const Context& options)
{
    if(request.contains(COMMAND))
    {
        return std::make_unique<QuitCommand>();
    }
    return std::nullopt;
}

}

}