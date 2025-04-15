#pragma once

#include "ICommand.hpp"

namespace SMTP
{

namespace Protocol
{

class QuitCommand final : public ICommand
{
public:
    constexpr static std::string_view COMMAND{"QUIT"sv};

    QuitCommand() = default;
    ~QuitCommand() = default;

    Response Execute(Context& options) override;
    static OptionalCommand TryParseCommand(const std::string& request, const Context& options);
};

}

}