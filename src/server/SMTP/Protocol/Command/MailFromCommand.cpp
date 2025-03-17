#include "MailFromCommand.hpp"

#include <print>

namespace SMTP
{

namespace Protocol
{

MailFromCommand::MailFromCommand(const Mailbox& mailbox)
    : m_mailbox{mailbox}
{
}

Response MailFromCommand::CreateResponse(Context& options)
{
    if(options.transaction.CheckAvailability(m_mailbox))
    {
        const Response response{ReplyCode::Ok, "Ok"};
        options.transaction.ChangeSender(m_mailbox);
        return response;
    }

    const Response response{ReplyCode::MailboxUnavailable, "Mailbox Unavailable", true};
    return response;
}

OptionalCommand MailFromCommand::TryParseCommand(const std::string& request, const Context& options)
{
    const auto command{std::data(COMMAND) + std::string{" FROM:"}};
    if(request.contains(command))
    {
        return std::make_unique<MailFromCommand>(Mailbox::ParseAddressFromRequest(request));
    }
    return std::nullopt;
}

}

}