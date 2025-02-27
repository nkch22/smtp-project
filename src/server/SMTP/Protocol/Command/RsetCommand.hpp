#pragma once

#include "ICommand.hpp"

namespace SMTP
{

namespace Protocol
{

class RsetCommand : public ICommand
{
public:
    constexpr static std::string_view COMMAND{"RSET"sv};

    RsetCommand() = default;
    ~RsetCommand() = default;
    Response CreateResponse(const Options& options) override;
};

}

}