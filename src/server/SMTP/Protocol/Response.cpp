#include "Response.hpp"

#include <format>

namespace SMTP
{

namespace Protocol
{

Response::Response(const ReplyCode reply_code, const OptionalMessage message)
    : m_reply_code{reply_code}, m_message{message}
{
}

ReplyCode Response::get_reply_code() const noexcept
{
    return m_reply_code;
}

Response::OptionalMessage Response::get_message() const noexcept
{
    return m_message;
}

std::string Response::CreateStringResponse() const noexcept
{
    if(!m_message.has_value())
    {
        return std::format("{}\r\n", to_underlying(get_reply_code()), get_message().value());
    }
    return std::format("{} {}\r\n", to_underlying(get_reply_code()), get_message().value());
}

}

}
