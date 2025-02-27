#include "EhloCommand.hpp"

#include <algorithm>
#include <format>

namespace SMTP
{

namespace Protocol
{

EhloCommand::EhloCommand(const std::string client_domain)
    : m_client_domain{std::move(client_domain)}
{
}

Response EhloCommand::CreateResponse(const Options& options)
{
    constexpr std::string_view extension_format{"250-{}\r\n"sv};
    std::string response_string{""};
    const auto extensions{FillExtensions(options)};
    for(const auto& extension : extensions)
    {
        response_string += std::format(extension_format, extension);
    }
    response_string += std::format("{} greets {}\r\n", options.domain_name, m_client_domain);
    const Response response{ReplyCode::Ok, response_string};
    return response;
}

std::vector<std::string> EhloCommand::FillExtensions(const Options& options) const
{
    std::vector<std::string> extensions{};
    extensions.emplace_back("PIPELING");
    extensions.emplace_back("8BITMIME");
    extensions.emplace_back("SMTPUTF8");
    if(!options.is_secure)
    {
        extensions.emplace_back("STARTTLS");
    }
    if(options.max_message_size > 0)
    {
        extensions.emplace_back(std::format("SIZE {}", options.max_message_size));
    }
    if(options.plain_login_allowed)
    {
        extensions.emplace_back("AUTH PLAIN LOGIN");
    }
    return extensions;
}

OptionalCommand EhloCommand::TryParseCommand(const std::string& request, const Options options)
{
    return std::nullopt;
}

}

}