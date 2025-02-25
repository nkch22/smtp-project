#pragma once

#include <string>

#include "ICommand.hpp"

namespace SMTP
{

namespace Protocol
{

class HeloCommand : public ICommand
{
public:
    constexpr static const char* COMMAND{"HELO"};

    HeloCommand() = default;
    ~HeloCommand() = default;
    Response CreateRespose() override;
}; 

}

}