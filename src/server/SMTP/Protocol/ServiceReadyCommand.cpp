#include "ServiceReadyCommand.hpp"

#include <format>

namespace SMTP
{
    namespace Protocol
    {
        Response ServiceReadyCommand::CreateResponse(const ServerContext& context)
        {
            Response response{ReplyCode::ServiceReady, ServiceReadyMessage(context)}; 
            return response;
        }

        std::string ServiceReadyCommand::ServiceReadyMessage(const ServerContext& context) const
        {
            return std::format("{} Simple Mail Transfer Service Ready", context.server_name);
        }
    }
}
