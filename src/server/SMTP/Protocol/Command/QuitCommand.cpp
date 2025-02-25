#include "QuitCommand.hpp"

#include <format>

namespace SMTP
{

namespace Protocol
{

Response QuitCommand::CreateRespose()
{
    const Response response{ReplyCode::Ok};
    return response;
}

}

}