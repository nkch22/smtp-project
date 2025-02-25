#include "AuthCommand.hpp"

namespace SMTP
{

namespace Protocol
{

Response AuthCommand::CreateRespose()
{
    const Response response{ReplyCode::Ok};
    return response;
}

}

}