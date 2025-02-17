#include "EhloCommand.hpp"

#include <format>

namespace SMTP
{
    namespace Protocol
    {
        EhloCommand::EhloCommand(const std::string& domain_or_address)
            : m_domain_of_address{domain_or_address}
        {
        }

        Response EhloCommand::CreateResponse(const ServerContext& context)
        {
            Response response{ReplyCode::Ok, CreateMessage(context)};
            return response;
        }

        std::string EhloCommand::CreateMessage(const ServerContext& context) const
        {
            return std::format("");
        }
    }
}