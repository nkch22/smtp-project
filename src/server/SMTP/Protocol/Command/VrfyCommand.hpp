#pragma once

#include "ICommand.hpp"

namespace SMTP
{

namespace Protocol
{

class VrfyCommand : public ICommand
{
public:
    constexpr static std::string_view COMMAND{"VRFY"sv};

    VrfyCommand() = default;
    ~VrfyCommand() = default;
    Response CreateResponse(const Options& options) override;
    static OptionalCommand TryParseCommand(const std::string& request, const Options& options);
};

}

}