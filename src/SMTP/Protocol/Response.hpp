#pragma once

#include <string>
#include <optional>

#include "ReplyCode.hpp"

namespace SMTP
{

namespace Protocol
{

class Response
{
public:
    using OptionalMessage = std::optional<std::string>;

    Response(const ReplyCode reply_code, const OptionalMessage message = std::nullopt, 
             const bool has_error = false, const bool add_crlf = true);
    Response(const Response& response) = default;
    Response& operator=(const Response& response) = default;
    Response(Response&& response) = default;
    Response& operator=(Response&& response) = default;

    ReplyCode GetReplyCode() const noexcept;
    OptionalMessage GetMessage() const noexcept;
    bool HasError() const noexcept;
    std::string CreateStringResponse() const noexcept;
private:
    ReplyCode m_reply_code;
    bool m_add_crlf;
    bool m_has_error;
    std::optional<std::string> m_message;
};

}

}