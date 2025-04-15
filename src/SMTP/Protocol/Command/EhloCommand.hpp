#pragma once

#include <vector>

#include "ICommand.hpp"

namespace SMTP
{

namespace Protocol
{

class EhloCommand final : public ICommand
{
public:
    constexpr static std::string_view COMMAND{"EHLO"sv};

    EhloCommand(const std::string client_domain);
    ~EhloCommand() = default;

    Response Execute(Context& options) override;
    static OptionalCommand TryParseCommand(const std::string& request, const Context options);
private:
    std::vector<std::string> FillExtensions(const Context& options) const;

    std::string m_client_domain;
};

}

}