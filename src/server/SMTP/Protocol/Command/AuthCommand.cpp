#include "AuthCommand.hpp"

namespace SMTP
{

namespace Protocol
{

Response AuthCommand::CreateResponse(const SessionContext& context)
{
    Response response{ReplyCode::Ok};
    return response;
}

}

}