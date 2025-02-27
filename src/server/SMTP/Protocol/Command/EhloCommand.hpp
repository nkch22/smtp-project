#pragma once

#include <vector>

#include "ICommand.hpp"
#include "../Options.hpp"

namespace SMTP
{

namespace Protocol
{

class EhloCommand : public ICommand
{
public:
    constexpr static std::string_view COMMAND{"EHLO"sv};

    EhloCommand(const std::string client_domain);
    ~EhloCommand() = default;
    Response CreateResponse(const Options& options) override;
private:
    std::vector<std::string> FillExtensions(const Options& options) const;
    std::string m_client_domain;
};

}

}