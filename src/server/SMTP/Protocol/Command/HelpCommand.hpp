#pragma once

#include "ICommand.hpp"

namespace SMTP
{

namespace Protocol
{

class HelpCommand : public ICommand
{
public:
    constexpr static std::string_view COMMAND{"HELP"};

    HelpCommand() = default;
    ~HelpCommand() = default;
    Response CreateResponse(const Options& options) override;
    static OptionalCommand TryParseCommand(const std::string& request, const Options& options);
};

}

}