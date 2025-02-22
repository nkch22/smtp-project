#pragma once

#include "ICommand.hpp"

namespace SMTP
{

namespace Protocol
{

class NoopCommand : public ICommand
{
public:
    constexpr static const char* COMMAND{"NOOP"};

    NoopCommand() = default;
    ~NoopCommand() = default;
    void Execute() override;
};

}

}