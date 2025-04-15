#include "EhloCommand.hpp"

#include <algorithm>
#include <format>

#include "Utils.hpp"

namespace SMTP
{

namespace Protocol
{

EhloCommand::EhloCommand(const std::string client_domain) : m_client_domain{std::move(client_domain)} {}

Response EhloCommand::Execute(Context& options)
{
	constexpr std::string_view extension_format{"250-{}\r\n"sv};
	const auto extensions{FillExtensions(options)};
	std::string response_string{""};
	response_string += std::format("{} greets {}\r\n", options.domain_name, m_client_domain);
	for (const auto& extension : extensions)
	{
		response_string += std::format(extension_format, extension);
	}
	const Response response{ReplyCode::Ok, response_string, false};
	return response;
}

std::vector<std::string> EhloCommand::FillExtensions(const Context& options) const
{
	std::vector<std::string> extensions{};
	extensions.emplace_back("8BITMIME");
	extensions.emplace_back("SMTPUTF8");
	if (options.max_message_size > 0)
	{
		extensions.emplace_back(std::format("SIZE {}", options.max_message_size));
	}
	if (options.plain_login_allowed)
	{
		extensions.emplace_back("AUTH PLAIN LOGIN");
	}
	return extensions;
}

OptionalCommand EhloCommand::TryParseCommand(const std::string& request, const Context options)
{
	if (const auto position{request.find(COMMAND)}; position != std::string::npos)
	{
		auto client_name{request.substr(position + std::size(COMMAND))};
		client_name = RemoveWhitespaces(client_name);
		return std::make_unique<EhloCommand>(client_name);
	}
	return std::nullopt;
}

}

}