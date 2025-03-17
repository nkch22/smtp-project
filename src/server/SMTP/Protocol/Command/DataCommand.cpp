#include "DataCommand.hpp"

namespace SMTP
{

namespace Protocol
{

DataCommand::DataCommand()
{
}

Response DataCommand::CreateResponse(Context& options)
{
    if(std::size(options.transaction.get_recepients()) == 0)
    {
        const Response response{ReplyCode::TransactionFailed, "No valid recepients given"};
        return response;
    }

    const Response response{ReplyCode::StartMailInput, "End data with <CR><LF>.<CR><LF>"};
    return response;
}

OptionalCommand DataCommand::TryParseCommand(const std::string& request, const Context& options)
{
    if(request.contains(COMMAND))
    {
        return std::make_unique<DataCommand>();
    }
    return std::nullopt;
}

}

}