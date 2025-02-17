#include "HeloCommand.hpp"

#include <format>

namespace SMTP
{
    namespace Protocol
    {
        Response HeloCommand::CreateResponse(const SessionContext& context)
        {
            Response response{ReplyCode::Ok, CreateMessage(context)};
            return response;
        }

        std::string HeloCommand::CreateMessage(const SessionContext& context) const
        {
            return std::format("{}", context.server_name);
        }
    }
}