#include "AuthCommand.hpp"

namespace SMTP
{

namespace Protocol
{

Response AuthCommand::CreateResponse(const Options& options)
{
    const Response response{ReplyCode::Ok};
    return response;
}

}

}