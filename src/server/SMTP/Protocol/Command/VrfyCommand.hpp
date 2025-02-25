#pragma once

#include "ICommand.hpp"

namespace SMTP
{

namespace Protocol
{

class VrfyCommand : public ICommand
{
public:
    constexpr static const char* COMMAND{"VRFY"};

    VrfyCommand() = default;
    ~VrfyCommand() = default;
    Response CreateRespose() override;
};

}

}