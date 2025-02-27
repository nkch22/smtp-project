#include "HelpCommand.hpp"

namespace SMTP
{

namespace Protocol
{

Response HelpCommand::CreateResponse(const Options& options)
{
    const Response response{Protocol::ReplyCode::Ok};
    return response;
}

}

}