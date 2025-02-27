#include "RsetCommand.hpp"

namespace SMTP
{

namespace Protocol
{

Response RsetCommand::CreateResponse(const Options& options)
{
    const Response response{ReplyCode::Ok};
    return response;
}

}

}