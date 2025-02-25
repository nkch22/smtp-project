#include "MailFromCommand.hpp"

namespace SMTP
{

namespace Protocol
{

Response MailFromCommand::CreateRespose()
{
    const Response response{ReplyCode::Ok};
    return response;
}

}

}