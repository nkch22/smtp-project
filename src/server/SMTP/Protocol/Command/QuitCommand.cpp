#include "QuitCommand.hpp"

#include <format>

namespace SMTP
{
    namespace Protocol
    {
        Response QuitCommand::CreateResponse(const SessionContext& context)
        {
            Response response{ReplyCode::ServiceClosingTransmissionChannel, ""};
            return response;
        }

        std::string QuitCommand::CreateMessage(const SessionContext& context) const
        {
            return std::format("{} Service closing transmission channel", context.hostname);
        }
    }
}