#include "RsetCommand.hpp"

namespace SMTP
{

namespace Protocol
{

Response RsetCommand::CreateRespose()
{
    const Response response{ReplyCode::Ok};
    return response;
}

}

}