#pragma once

#include "ICommand.hpp"

namespace SMTP
{

namespace Protocol
{

class EhloCommand : public ICommand
{
public:
    constexpr static const char* COMMAND{"EHLO"};

    EhloCommand() = default;
    ~EhloCommand() = default;
    Response CreateRespose() override;
private:
    std::string m_domain_of_address;
};

}

}