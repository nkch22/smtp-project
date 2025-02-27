#pragma once

#include "ICommand.hpp"

namespace SMTP
{

namespace Protocol
{

class QuitCommand : public ICommand
{
public:
    constexpr static std::string_view COMMAND{"QUIT"sv};

    QuitCommand() = default;
    ~QuitCommand() = default;
    Response CreateResponse(const Options& options) override;
    static OptionalCommand TryParseCommand(const std::string& request, const Options& options);
};

}

}