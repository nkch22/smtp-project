#pragma once

#include <string>

#include "ICommand.hpp"

namespace SMTP
{

namespace Protocol
{

class HeloCommand final : public ICommand
{
public:
    constexpr static std::string_view COMMAND{"HELO"sv};

    HeloCommand(const std::string client_domain);
    ~HeloCommand() = default;

    Response Execute(Context& options) override;
    static OptionalCommand TryParseCommand(const std::string& request, const Context& options);
private:
    static std::string RemoveWhitespaces(const std::string& string);

    std::string m_client_domain;
}; 

}

}