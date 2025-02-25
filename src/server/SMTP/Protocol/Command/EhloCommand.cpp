#include "EhloCommand.hpp"

#include <format>

namespace SMTP
{

namespace Protocol
{

Response EhloCommand::CreateRespose()
{
    const Response response{ReplyCode::Ok};
    return response;
}

}

}