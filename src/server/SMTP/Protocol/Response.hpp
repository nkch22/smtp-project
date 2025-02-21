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

    Response(const ReplyCode reply_code, const OptionalMessage message = std::nullopt);
    ReplyCode get_reply_code() const noexcept;
    OptionalMessage get_message() const noexcept;
    std::string CreateStringResponse() const noexcept;
private:
    ReplyCode m_reply_code;
    std::optional<std::string> m_message;
};

}

}