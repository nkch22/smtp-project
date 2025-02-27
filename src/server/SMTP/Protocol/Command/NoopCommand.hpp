#pragma once

#include "ICommand.hpp"

namespace SMTP
{

namespace Protocol
{

class NoopCommand : public ICommand
{
public:
    constexpr static std::string_view COMMAND{"NOOP"sv};

    NoopCommand() = default;
    ~NoopCommand() = default;
    Response CreateResponse(const Options& options) override;
};

}

}