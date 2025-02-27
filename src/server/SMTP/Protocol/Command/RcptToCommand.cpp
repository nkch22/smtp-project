#include "RcptToCommand.hpp"

namespace SMTP
{

namespace Protocol
{

Response RcptToCommand::CreateResponse(const Options& options)
{
    const Response response{ReplyCode::Ok};
    return response;
}

}

}