#pragma once

#include "ICommand.hpp"

namespace SMTP
{

namespace Protocol
{

class AuthCommand : public ICommand
{
public:
    constexpr static const char* COMMAND{"AUTH"};

    AuthCommand() = default;
    ~AuthCommand() = default;
    Response CreateRespose() override;
};

}

}