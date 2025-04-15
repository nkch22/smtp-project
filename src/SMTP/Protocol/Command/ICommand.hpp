#pragma once

#include <memory>
#include <string_view>

#include "../Response.hpp"
#include "../../Context.hpp"

using namespace std::string_view_literals;

namespace SMTP
{
    
namespace Protocol
{

class ICommand
{
public:
    virtual ~ICommand() = default;
    virtual Response Execute(Context& options) = 0;
};

using OptionalCommand = std::optional<std::unique_ptr<ICommand>>;

}

}