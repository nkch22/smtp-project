#pragma once

#include "ICommand.hpp"

namespace SMTP
{

namespace Protocol
{

class VrfyCommand final : public ICommand
{
public:
    constexpr static std::string_view COMMAND{"VRFY"sv};

    VrfyCommand() = default;
    ~VrfyCommand() = default;

    Response Execute(Context& options) override;
    static OptionalCommand TryParseCommand(const std::string& request, const Context& options);
};

}

}