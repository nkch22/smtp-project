#pragma once

#include <optional>
#include <memory>
#include <mutex>
#include <vector>

#include "../Context.hpp"
#include "ICommand.hpp"

namespace SMTP
{

namespace Protocol
{

class Parser
{
public:
    Parser() = default;
    ~Parser() = default;
    
    OptionalCommand TryParseRequest(const std::string& request, const Context& options) const;
};

}

}