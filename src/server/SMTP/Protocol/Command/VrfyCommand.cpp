#include "VrfyCommand.hpp"

namespace SMTP
{

namespace Protocol
{

Response VrfyCommand::CreateResponse(const Options& options)
{
    const Response response{ReplyCode::Ok};
    return response;
}

}

}