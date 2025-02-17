#include "QuitCommand.hpp"

#include <format>

namespace SMTP
{
    namespace Protocol
    {
        Response QuitCommand::CreateResponse(const ServerContext& context)
        {
            Response response{ReplyCode::ServiceClosingTransmissionChannel, ""};
            return response;
        }

        std::string QuitCommand::CreateMessage(const ServerContext& context) const
        {
            return std::format("{} Service closing transmission channel", context.server_name);
        }
    }
}