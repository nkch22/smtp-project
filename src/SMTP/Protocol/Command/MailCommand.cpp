#include "MailCommand.hpp"

namespace SMTP
{

namespace Protocol
{

MailCommand::MailCommand(const Mailbox& mailbox) : m_mailbox{mailbox} {}

Response MailCommand::Execute(Context& options)
{
	if (options.transaction.CheckAvailability(m_mailbox))
	{
		const Response response{ReplyCode::Ok, "Ok"};
		options.transaction.ChangeSender(m_mailbox);
		return response;
	}

	const Response response{ReplyCode::MailboxUnavailable, "Mailbox Unavailable", true};
	return response;
}

OptionalCommand MailCommand::TryParseCommand(const std::string& request, const Context& options)
{
	const auto command{std::data(COMMAND) + std::string{" FROM:"}};
	if (request.find(COMMAND) != std::string::npos)
	{
		return std::make_unique<MailCommand>(Mailbox::ParseAddressFromRequest(request));
	}
	return std::nullopt;
}

}

}