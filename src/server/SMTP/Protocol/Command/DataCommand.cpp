#include "DataCommand.hpp"

namespace SMTP
{

namespace Protocol
{

Response DataCommand::CreateRespose()
{
    const Response response{ReplyCode::Ok};
    return response;
}

}

}