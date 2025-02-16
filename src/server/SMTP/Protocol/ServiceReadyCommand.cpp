#include "ServiceReadyCommand.hpp"

#include <format>

namespace SMTP
{
    namespace Protocol
    {
        Response ServiceReadyCommand::CreateResponse(const CommandContext& context)
        {
            Response response{ReplyCode::ServiceReady, ServiceReadyMessage(context)}; 
            return response;
        }

        std::string ServiceReadyCommand::ServiceReadyMessage(const CommandContext& context) const
        {
            return std::format("{} Simple Mail Transfer Service Ready", context.get_server_name());
        }
    }
}
