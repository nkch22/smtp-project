#include "RsetCommand.hpp"

namespace SMTP
{
    namespace Protocol
    {
        Response RsetCommand::CreateResponse(const SessionContext& context)
        {
            Response response{ReplyCode::Ok};
            return response;
        }
    }
}