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
    constexpr static std::string_view COMMAND{"HELO"sv};

    HeloCommand(const std::string client_domain);
    ~HeloCommand() = default;
    Response CreateResponse(const Options& options) override;
private:
    std::string m_client_domain;
}; 

}

}