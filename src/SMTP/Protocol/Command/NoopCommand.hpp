#pragma once

#include "ICommand.hpp"

namespace SMTP
{

namespace Protocol
{

class NoopCommand final : public ICommand
{
public:
    constexpr static std::string_view COMMAND{"NOOP"sv};

    NoopCommand() = default;
    ~NoopCommand() = default;

    Response Execute(Context& options) override;
    static OptionalCommand TryParseCommand(const std::string& request, const Context& options);
};

}

}