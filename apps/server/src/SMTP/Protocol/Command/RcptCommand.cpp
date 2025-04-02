#include "RcptCommand.hpp"

namespace SMTP
{

namespace Protocol
{

RcptCommand::RcptCommand(const Mailbox& mailbox) : m_mailbox{mailbox} {}

Response RcptCommand::CreateResponse(Context& options)
{
	if (options.transaction.CheckAvailability(m_mailbox))
	{
		const Response response{ReplyCode::Ok, "Ok"};
		options.transaction.AddRecepient(m_mailbox);
		return response;
	}

	const Response response{ReplyCode::MailboxUnavailable, "Mailbox Unavailable", true};
	return response;
}

OptionalCommand RcptCommand::TryParseCommand(const std::string& request, const Context& options)
{
	const auto command{std::data(COMMAND) + std::string{" TO:"}};
	if (request.find(COMMAND) != std::string::npos)
	{
		return std::make_unique<RcptCommand>(Mailbox::ParseAddressFromRequest(request));
	}
	return std::nullopt;
}

} // namespace Protocol

} // namespace SMTP