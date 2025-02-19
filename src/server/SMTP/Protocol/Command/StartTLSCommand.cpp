#include "StartTLSCommand.hpp"

namespace SMTP
{
    namespace Protocol
    {
        Response StartTLSCommand::CreateResponse(const SessionContext& context)
        {
            Response response{ReplyCode::ServiceReady}; 
            return response;
        }
    }
}