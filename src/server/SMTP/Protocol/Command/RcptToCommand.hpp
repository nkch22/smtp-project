#pragma once

#include "ICommand.hpp"

namespace SMTP
{

namespace Protocol
{

class RcptToCommand : public ICommand
{
public:
    constexpr static std::string_view COMMAND{"RCPT TO"sv};

    RcptToCommand() = default;
    ~RcptToCommand() = default;
    Response CreateResponse(const Options& options) override;
    static OptionalCommand TryParseCommand(const std::string& request, const Options& options);
};

}

}