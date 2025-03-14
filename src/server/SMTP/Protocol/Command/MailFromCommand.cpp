#include "MailFromCommand.hpp"

namespace SMTP
{

namespace Protocol
{

Response MailFromCommand::CreateResponse(const Options& options)
{
	const Response response{ReplyCode::Ok};
	return response;
}

OptionalCommand MailFromCommand::TryParseCommand(const std::string& request, const Options& options)
{
	if (request.find(COMMAND) != std::string::npos)
	{
		return std::make_unique<MailFromCommand>();
	}
	return std::nullopt;
}

} // namespace Protocol

} // namespace SMTP