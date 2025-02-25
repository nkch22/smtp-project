#pragma once

#include "ICommand.hpp"

namespace SMTP
{

namespace Protocol
{

class DataCommand : public ICommand
{
public:
    constexpr static const char* COMMAND{"DATA"};

    DataCommand() = default;
    ~DataCommand() = default;
    Response CreateRespose() override;
};

}

}