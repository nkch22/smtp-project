#pragma once

#include "ICommand.hpp"

namespace SMTP
{

namespace Protocol
{

class DataCommand : public ICommand
{
public:
    constexpr static std::string_view COMMAND{"DATA"sv};

    DataCommand() = default;
    ~DataCommand() = default;
    Response CreateResponse(const Options& options) override;
};

}

}