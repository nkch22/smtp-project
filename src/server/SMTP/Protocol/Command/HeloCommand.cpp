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

Response HeloCommand::CreateResponse(Context& options)
{
    const Response response{ReplyCode::Ok, 
        std::format("{} greets {}", options.domain_name, m_client_domain)};
    return response;
}

OptionalCommand HeloCommand::TryParseCommand(const std::string& request, const Context& options)
{
    if(const auto position{request.find(HeloCommand::COMMAND)};
       position != std::string::npos)
    {
        auto client_name{request.substr(position + std::size(HeloCommand::COMMAND))};
        client_name = RemoveWhitespaces(client_name);
        return std::make_unique<HeloCommand>(client_name);
    }
    return std::nullopt;
}

std::string HeloCommand::RemoveWhitespaces(const std::string& string)
{
    auto copy{string};
    auto removed_elements{std::remove_if(std::begin(copy), std::end(copy), ::isspace)};
    copy.erase(removed_elements, std::end(copy));
    return copy;
}

}

}