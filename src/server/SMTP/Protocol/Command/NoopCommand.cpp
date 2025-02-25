#include "NoopCommand.hpp"

namespace SMTP
{

namespace Protocol
{

Response NoopCommand::CreateRespose()
{
    const Response response{ReplyCode::Ok};
    return response;
}

}

}