#pragma once

#include "ICommand.hpp"

namespace SMTP
{

namespace Protocol
{

class QuitCommand : public ICommand
{
public:
    constexpr static const char* COMMAND{"QUIT"};

    QuitCommand() = default;
    ~QuitCommand() = default;
    void Execute() override;
};

}

}