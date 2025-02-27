#pragma once

#include "ICommand.hpp"

namespace SMTP
{

namespace Protocol
{

class AuthCommand : public ICommand
{
public:
    constexpr static std::string_view COMMAND{"AUTH"sv};

    AuthCommand() = default;
    ~AuthCommand() = default;
    Response CreateResponse(const Options& options) override;
    static OptionalCommand TryParseCommand(const std::string& request, const Options& options);
};

}

}