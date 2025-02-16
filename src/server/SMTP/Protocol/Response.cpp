#include "Response.hpp"

namespace SMTP
{
    namespace Protocol
    {
        Response::Response(const ReplyCode reply_code, const Message message)
            : m_reply_code{reply_code}, m_message{message}
        {
        }

        ReplyCode Response::get_reply_code() const noexcept
        {
            return m_reply_code;
        }

        Response::Message Response::get_message() const noexcept
        {
            return m_message;
        }

        /*
        const Response Response::OK{ReplyCode::Ok, "Ok"};
        const Response Response::SERVICE_READY{ReplyCode::ServiceReady, "ready when you are"};
        const Response Response::MAILBOX_UNAVAILABLE{ReplyCode::MailboxUnavailable, "mailbox unavailable"};
        const Response Response::MAILBOX_NAME_NOT_ALLOWED{ReplyCode::MailboxNameNotAllowed, "mailbox name not allowed"};
        const Response Response::SERVICE_CLOSING_TRANSMISSION_CHANNEL{ReplyCode::ServiceClosingTransmissionChannel, "bye"};
        const Response Response::SYNTAX_ERROR{ReplyCode::SyntaxError, "syntax error"};
        const Response Response::SIZE_LIMIT_EXCEEDED{ReplyCode::SizeLimitExceeded, "size limit exceeded"};
        const Response Response::NO_VALID_RECIPIENTS_GIVEN{ReplyCode::TransactionFailed, "no valid recipients given"};
        const Response Response::AUTHENTICATION_FAILED{ReplyCode::AuthenticationFailed, "authentification failed"};
        const Response Response::AUTHENTICATION_SUCCESSFULL{ReplyCode::AuthenticationSuccessful, "go ahead"};
        const Response Response::TRANSACTION_FAILED{ReplyCode::TransactionFailed};
        const Response Response::BAD_SEQUENCE{ReplyCode::BadSequence, "bad sequence commands"};
        const Response Response::AUTHENTIFICATION_REQUIRED{ReplyCode::AuthenticationRequired, "authentification required"};
        */
    }
}
