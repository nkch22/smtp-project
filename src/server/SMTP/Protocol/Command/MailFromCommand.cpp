#include "MailFromCommand.hpp"

namespace SMTP
{
    namespace Protocol
    {
        Response MailFromCommand::CreateResponse(const SessionContext& context)
        {
            Response response{ReplyCode::Ok};
            return response;
        }
    }
}