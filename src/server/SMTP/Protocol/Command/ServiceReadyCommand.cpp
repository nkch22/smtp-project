#include "ServiceReadyCommand.hpp"

#include <format>

namespace SMTP
{
    namespace Protocol
    {
        Response ServiceReadyCommand::CreateResponse(const SessionContext& context)
        {
            Response response{ReplyCode::ServiceReady, ServiceReadyMessage(context)}; 
            return response;
        }

        std::string ServiceReadyCommand::ServiceReadyMessage(const SessionContext& context) const
        {
            return std::format("{} Simple Mail Transfer Service Ready", context.hostname);
        }
    }
}
