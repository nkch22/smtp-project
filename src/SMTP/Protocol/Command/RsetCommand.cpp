#include "RsetCommand.hpp"

namespace SMTP
{

namespace Protocol
{

Response RsetCommand::Execute(Context& options)
{
	const Response response{ReplyCode::Ok};
	return response;
}

OptionalCommand RsetCommand::TryParseCommand(const std::string& request, const Context& options)
{
	if (request.find(COMMAND) != std::string::npos)
	{
		return std::make_unique<RsetCommand>();
	}
	return std::nullopt;
}

}

}