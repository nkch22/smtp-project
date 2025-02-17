#include "DataCommand.hpp"

namespace SMTP
{
    namespace Protocol
    {
        Response DataCommand::CreateResponse(const SessionContext& context)
        {
            Response response{ReplyCode::Ok};
            return response;
        }
    }
}