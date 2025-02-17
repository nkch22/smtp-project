#include "RcptToCommand.hpp"

namespace SMTP
{
    namespace Protocol
    {
        Response RcptToCommand::CreateResponse(const SessionContext& context)
        {
            Response response{ReplyCode::Ok};
            return response;
        }
    }
}