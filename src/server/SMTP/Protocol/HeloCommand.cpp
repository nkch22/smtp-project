#include "HeloCommand.hpp"

#include <format>

namespace SMTP
{
    namespace Protocol
    {
        HeloCommand::HeloCommand(const std::string& domain_or_address)
            : m_domain_of_address{domain_or_address}
        {
        }

        Response HeloCommand::CreateResponse(const ServerContext& context)
        {
            Response response{ReplyCode::Ok, CreateMessage(context)};
            return response;
        }

        std::string HeloCommand::CreateMessage(const ServerContext& context) const
        {
            return std::format("{} Hello {}", context.server_name, m_domain_of_address);
        }
    }
}