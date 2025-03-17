#include "RcptToCommand.hpp"

namespace SMTP
{

namespace Protocol
{

RcptToCommand::RcptToCommand(const Mailbox& mailbox)
    : m_mailbox{mailbox}
{
}

Response RcptToCommand::CreateResponse(Context& options)
{
    if(options.transaction.CheckAvailability(m_mailbox))
    {
        const Response response{ReplyCode::Ok, "Ok"};
        options.transaction.AddRecepient(m_mailbox);
        return response;
    }

    const Response response{ReplyCode::MailboxUnavailable, "Mailbox Unavailable", true};
    return response;
}

OptionalCommand RcptToCommand::TryParseCommand(const std::string& request, const Context& options)
{
    const auto command{std::data(COMMAND) + std::string{" TO:"}};
    if(request.contains(command))
    {
        return std::make_unique<RcptToCommand>(Mailbox::ParseAddressFromRequest(request));
    }
    return std::nullopt;
}

}

}