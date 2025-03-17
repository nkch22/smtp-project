#include "AuthCommand.hpp"

namespace SMTP
{

namespace Protocol
{

AuthCommand::AuthCommand(const AuthentificationMethod auth_method, const std::string_view parameter)
    : m_auth_method{auth_method}, m_parameter{parameter}
{
}

Response AuthCommand::CreateResponse(Context& options)
{
    using enum AuthentificationMethod;

    const Response response{ReplyCode::Ok};
    switch(m_auth_method)
    {
    case Plain:
        break;
    case Login:
        break;
    }
    return response;
}

OptionalCommand AuthCommand::TryParseCommand(const std::string& request, const Context& options)
{
    if(request.contains(COMMAND))
    {
        //return std::make_unique<AuthCommand>();
    }
    return std::nullopt;
}

std::optional<Response> AuthCommand::TryPlain(const Context& options)
{
    return std::nullopt;
}

std::optional<Response> AuthCommand::TryLogin(const Context& options)
{
    return std::nullopt;
}

}

}