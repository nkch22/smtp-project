#include "NoopCommand.hpp"

namespace SMTP
{

namespace Protocol
{

Response NoopCommand::CreateResponse(const Options& options)
{
    const Response response{ReplyCode::Ok};
    return response;
}

}

}