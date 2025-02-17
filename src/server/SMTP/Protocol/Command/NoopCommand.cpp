#include "NoopCommand.hpp"

namespace SMTP
{
    namespace Protocol
    {
        Response NoopCommand::CreateResponse(const SessionContext& context)
        {
            Response response{ReplyCode::Ok};
            return response;
        }
    }
}