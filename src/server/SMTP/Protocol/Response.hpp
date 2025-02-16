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
            using Message = std::optional<std::string>;
            
            /*
            static const Response OK;
            static const Response SERVICE_READY;
            static const Response MAILBOX_UNAVAILABLE;
            static const Response MAILBOX_NAME_NOT_ALLOWED;
            static const Response SERVICE_CLOSING_TRANSMISSION_CHANNEL;
            static const Response SYNTAX_ERROR;
            static const Response SIZE_LIMIT_EXCEEDED;
            static const Response NO_VALID_RECIPIENTS_GIVEN;
            static const Response AUTHENTICATION_FAILED;
            static const Response AUTHENTICATION_SUCCESSFULL;
            static const Response TRANSACTION_FAILED;
            static const Response BAD_SEQUENCE;
            static const Response AUTHENTIFICATION_REQUIRED;
            */

            Response(const ReplyCode reply_code, const Message message = std::nullopt);
            ReplyCode get_reply_code() const noexcept;
            Message get_message() const noexcept;
        private:
            ReplyCode m_reply_code;
            std::optional<std::string> m_message;
        };
    }
}