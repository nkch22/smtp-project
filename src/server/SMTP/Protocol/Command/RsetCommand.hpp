#pragma once

#include "ICommand.hpp"

namespace SMTP
{

namespace Protocol
{

class RsetCommand : public ICommand
{
public:
    constexpr static const char* COMMAND{"RSET"};

    RsetCommand() = default;
    ~RsetCommand() = default;
    void Execute() override;
};

}

}