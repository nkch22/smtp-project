#pragma once

#include "ICommand.hpp"

namespace SMTP
{

namespace Protocol
{

class HelpCommand final : public ICommand
{
public:
    constexpr static std::string_view COMMAND{"HELP"};

    HelpCommand() = default;
    ~HelpCommand() = default;

    Response Execute(Context& options) override;
    static OptionalCommand TryParseCommand(const std::string& request, const Context& options);
private:
    static std::string FormatData(const std::string_view data);
};
    
}

}