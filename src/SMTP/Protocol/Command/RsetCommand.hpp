#pragma once

#include "ICommand.hpp"

namespace SMTP
{

namespace Protocol
{

class RsetCommand final : public ICommand
{
public:
    constexpr static std::string_view COMMAND{"RSET"sv};

    RsetCommand() = default;
    ~RsetCommand() = default;

    Response Execute(Context& options) override;
    static OptionalCommand TryParseCommand(const std::string& request, const Context& options);
};

}

}