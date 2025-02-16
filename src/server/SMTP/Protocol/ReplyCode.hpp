#pragma once

#include <type_traits>

namespace SMTP
{
    namespace Protocol
    {
        enum class ReplyCode
        {
            // The server is unable to connect.
            UnableToConnect = 101,

            // Connection refused or inability to open an SMTP stream.
            ConnectionRefused = 111,

            // System status message or help reply.
            SystemMessage = 211,

            // A response to the HELP command.
            HelpResponse = 214,

            // The service is ready.
            ServiceReady = 220,

            // Goodbye.
            ServiceClosingTransmissionChannel = 221,

            // Authentication was Successful.
            AuthenticationSuccessful = 235,

            // Everything was Ok.
            Ok = 250,

            // "User not local will forward": the recipient’s account is not on the present server, so it will be relayed to another.
            RelayToAnotherServer = 251,

            // The server cannot verify the user, but it will try to deliver the message anyway.
            CantVerifyUser = 252,

            // Continue with the authentication.
            ContinueWithAuth = 334,

            // Start the mail input.
            StartMailInput = 354,

            // "Timeout connection problem": there have been issues during the message transfer.
            TimeoutConnectionProblem = 420,

            // The service is unavailable due to a connection problem: it may refer to an exceeded limit of simultaneous connections, or a more general temporary problem.
            // The server (yours or the recipient's) is not available at the moment, so the dispatch will be tried again later.
            ServiceUnavailable = 421,

            // The recipient’s mailbox has exceeded its storage limit.
            ExceededStorage = 422,

            // Not enough space on the disk, or an "out of memory" condition due to a file overload.
            Overloaded = 431,

            // The recipient’s server is not responding.
            RecipientNotResponding = 441,

            // The connection was dropped during the transmission.
            ConnectionDropped = 442,

            // The maximum hop count was exceeded for the message: an internal loop has occurred.
            MaxHopCountExceeded = 446,

            // Your outgoing message timed out because of issues concerning the incoming server.
            MessageTimeout = 447,

            // A routing error.
            RoutingError = 449,

            // "Requested action not taken – The user’s mailbox is unavailable". The mailbox has been corrupted or placed on an offline server, or your email hasn't been accepted for IP problems or blacklisting.
            Unavailable = 450,

            // "Requested action aborted – Local error in processing". Your ISP's server or the server that got a first relay from yours has encountered a connection problem.
            Aborted = 451,

            // There is insufficent stored to handle the mail.
            InsufficientStorage = 452,

            // The client is not permitted to connect.
            ClientNotPermitted = 454,

            // An error of your mail server, often due to an issue of the local anti-spam filter.
            Error = 471,

            // Syntax error, command unrecognized (This may include errors such as command line too long).
            CommandUnrecognized = 500,

            // Syntax error in parameters or arguments.
            SyntaxError = 501,

            // The command has not been implemented.
            CommandNotImplemented = 502,

            // Bad sequence of commands.
            BadSequence = 503,

            // A command parameter is not implemented.
            CommandParameterNotImplemented = 504,

            // Bad email address.
            // Codes 510 or 511 result the same structure.
            // One of the addresses in your TO, CC or BBC line doesn't exist. Check again your recipients' accounts and correct any possible misspelling.
            BadEmailAddress = 510,

            // A DNS error: the host server for the recipient's domain name cannot be found.
            DnsError = 512,

            // "Address type is incorrect": another problem concerning address misspelling. In few cases, however, it's related to an authentication issue.
            IncorrectAddressType = 513,

            // The total size of your mailing exceeds the recipient server's limits.
            MailingLimitExceeded = 523,

            // Authentication required
            AuthenticationRequired = 530,

            // Authentication failed.
            AuthenticationFailed = 535,

            // The recipient address rejected your message: normally, it's an error caused by an anti-spam filter.
            RecipientAddressRejected = 541,

            // The Mailbox is temporarily unavailable.
            MailboxUnavailable = 550,

            // "User not local or invalid address – Relay denied". Meaning, if both your address and the recipients are not locally hosted by the server, a relay can be interrupted.
            RelayDenied = 551,

            // The size limit has been exceeded.
            SizeLimitExceeded = 552,

            // The Mailbox is permanently not available.
            MailboxNameNotAllowed = 553,

            // The transaction failed.
            TransactionFailed = 554
        };

        constexpr std::underlying_type_t<ReplyCode> to_underlying(const ReplyCode reply_code) noexcept 
        {
            return static_cast<std::underlying_type_t<ReplyCode>>(reply_code);
        }
    }
}
