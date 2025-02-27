#pragma once

#include "ICommand.hpp"

namespace SMTP
{

namespace Protocol
{

class MailFromCommand : public ICommand
{
public:
    constexpr static std::string_view COMMAND{"MAIL FROM"sv};

    MailFromCommand() = default;
    ~MailFromCommand() = default;
    Response CreateResponse(const Options& options) override;
    static OptionalCommand TryParseCommand(const std::string& request, const Options& options);
};

}

}