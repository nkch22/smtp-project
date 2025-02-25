#include "VrfyCommand.hpp"

namespace SMTP
{

namespace Protocol
{

Response VrfyCommand::CreateRespose()
{
    const Response response{ReplyCode::Ok};
    return response;
}

}

}