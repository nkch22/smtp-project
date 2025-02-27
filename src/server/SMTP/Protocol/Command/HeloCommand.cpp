#include "HeloCommand.hpp"

#include <format>

namespace SMTP
{

namespace Protocol
{

HeloCommand::HeloCommand(const std::string client_domain)
    : m_client_domain{std::move(client_domain)}
{
}

Response HeloCommand::CreateResponse(const Options& options)
{
    const Response response{ReplyCode::Ok, 
        std::format("{} greets {}", options.domain_name, m_client_domain)};
    return response;
}

OptionalCommand HeloCommand::TryParseCommand(const std::string& request, const Options& options)
{
    return std::nullopt;
}

}

}