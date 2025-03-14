#include "RcptToCommand.hpp"

namespace SMTP
{

namespace Protocol
{

Response RcptToCommand::CreateResponse(const Options& options)
{
	const Response response{ReplyCode::Ok};
	return response;
}

OptionalCommand RcptToCommand::TryParseCommand(const std::string& request, const Options& options)
{
	if (request.find(COMMAND) != std::string::npos)
	{
		return std::make_unique<RcptToCommand>();
	}
	return std::nullopt;
}

} // namespace Protocol

} // namespace SMTP