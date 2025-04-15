#pragma once

#include "ICommand.hpp"
#include "../../Mailbox.hpp"

namespace SMTP
{

namespace Protocol
{

class MailCommand final : public ICommand
{
public:
    constexpr static std::string_view COMMAND{"MAIL"sv};
    constexpr static std::string_view END_OF_MAIL{"\r\n.\r\n"sv};

    MailCommand(const Mailbox& mailbox);
    ~MailCommand() = default;

    Response Execute(Context& options) override;
    static OptionalCommand TryParseCommand(const std::string& request, const Context& options);
private:
    Mailbox m_mailbox;
};

}

}