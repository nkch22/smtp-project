#include "HeloCommand.hpp"

#include <format>

namespace SMTP
{

namespace Protocol
{

Response HeloCommand::CreateRespose()
{
    const Response response{ReplyCode::Ok};
    return response;
}

}

}