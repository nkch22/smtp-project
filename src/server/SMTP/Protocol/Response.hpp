#pragma once
/**
 * @file Response.hpp
 * @author Oleksandr
 * @brief 
 * @version 0.1
 * @date 2025-03-04
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <string>
#include <optional>

#include "ReplyCode.hpp"

namespace SMTP
{

namespace Protocol
{

/**
 * @brief Class Response that contains response message
 * 
 */
class Response
{
public:
    using OptionalMessage = std::optional<std::string>;

    /**
     * @brief Construct a new Response object
     * 
     * @param reply_code 
     * @param message 
     * @param add_crlf 
     */
    Response(const ReplyCode reply_code, const OptionalMessage message = std::nullopt, 
             const bool has_error = false, const bool add_crlf = true);

    /**
     * @brief Get the reply code object
     * 
     * @return ReplyCode 
     */
    ReplyCode get_reply_code() const noexcept;

    /**
     * @brief Get the message object
     * 
     * @return OptionalMessage 
     */
    OptionalMessage get_message() const noexcept;

    bool HasError() const noexcept;

    /**
     * @brief Create a String Response object
     * 
     * @return std::string 
     */
    std::string CreateStringResponse() const noexcept;
private:
    ReplyCode m_reply_code;
    bool m_add_crlf;
    bool m_has_error;
    std::optional<std::string> m_message;
};

}

}