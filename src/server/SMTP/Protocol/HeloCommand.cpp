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

        Response HeloCommand::CreateResponse(const CommandContext& context)
        {
            Response response{ReplyCode::Ok, CreateGreetingMessage(context)};
            return response;
        }

        std::string HeloCommand::CreateGreetingMessage(const CommandContext& context) const
        {
            return std::format("{} Hello {}", context.get_server_name(), m_domain_of_address);
        }
    }
}