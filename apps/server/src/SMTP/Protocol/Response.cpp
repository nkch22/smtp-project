#include "Response.hpp"
/**
 * @file Response.cpp
 * @author Oleksandr (olexandrfedorych@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2025-03-04
 * 
 * @copyright Copyright (c) 2025
 * 
 */
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
    std::string string_response{};
    if(m_message.has_value())
    {
        string_response = std::format("{} {}", to_underlying(get_reply_code()), get_message().value());
    }
    else
    {
        string_response = std::format("{}", to_underlying(get_reply_code()));
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
