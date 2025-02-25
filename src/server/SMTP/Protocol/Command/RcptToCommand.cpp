#include "RcptToCommand.hpp"

namespace SMTP
{

namespace Protocol
{

Response RcptToCommand::CreateRespose()
{
    const Response response{ReplyCode::Ok};
    return response;
}

}

}