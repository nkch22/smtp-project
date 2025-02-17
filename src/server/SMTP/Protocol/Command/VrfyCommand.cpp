#include "VrfyCommand.hpp"

namespace SMTP
{
    namespace Protocol
    {
        Response VrfyCommand::CreateResponse(const SessionContext& context)
        {
            Response response{ReplyCode::Ok};
            return response;
        }
    }
}