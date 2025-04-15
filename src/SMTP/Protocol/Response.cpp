#include "Response.hpp"

#include <format>

namespace SMTP
{

namespace Protocol
{

Response::Response(const ReplyCode reply_code, const OptionalMessage message, const bool has_error, const bool add_crlf)
    : m_reply_code{reply_code}
    , m_message{message}
    , m_has_error{has_error}
    , m_add_crlf{add_crlf}
{
}

ReplyCode Response::GetReplyCode() const noexcept
{
    return m_reply_code;
}

Response::OptionalMessage Response::GetMessage() const noexcept
{
    return m_message;
}

std::string Response::CreateStringResponse() const noexcept
{
    std::string string_response{};
    if(m_message.has_value())
    {
        string_response = std::format("{} {}", to_underlying(GetReplyCode()), GetMessage().value());
    }
    else
    {
        string_response = std::format("{}", to_underlying(GetReplyCode()));
    }
    if(m_add_crlf)
    {
        return std::format("{}\r\n", string_response);
    }
    return std::format("{}", string_response);
}

bool Response::HasError() const noexcept
{
    return m_has_error;
}

}

}
