#pragma once

#include "ICommand.hpp"
#include "../../Mailbox.hpp"

namespace SMTP
{

namespace Protocol
{

class RcptCommand final : public ICommand
{
public:
    constexpr static std::string_view COMMAND{"RCPT"sv};

    RcptCommand(const Mailbox& mailbox);
    ~RcptCommand() = default;

    Response Execute(Context& options) override;
    static OptionalCommand TryParseCommand(const std::string& request, const Context& options);
private:
    Mailbox m_mailbox;
};

}

}