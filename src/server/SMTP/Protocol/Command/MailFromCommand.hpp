#pragma once

#include "ICommand.hpp"

namespace SMTP
{

namespace Protocol
{

class MailFromCommand : public ICommand
{
public:
    constexpr static const char* COMMAND{"MAIL FROM"};

    MailFromCommand() = default;
    ~MailFromCommand() = default;
    void Execute() override;
};

}

}