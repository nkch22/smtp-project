#pragma once

#include "ICommand.hpp"

namespace SMTP
{

namespace Protocol
{

class DataCommand final : public ICommand
{
public:
    constexpr static std::string_view COMMAND{"DATA"sv};

    DataCommand();
    ~DataCommand() = default;

    Response Execute(Context& options) override;
    static OptionalCommand TryParseCommand(const std::string& request, const Context& options);
};

}

}