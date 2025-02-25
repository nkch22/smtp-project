#pragma once

#include <memory>

#include "../Response.hpp"
#include "../../ISession.hpp"

namespace SMTP
{

namespace Protocol
{

class ICommand
{
public:
    virtual ~ICommand() = default;
    virtual Response CreateRespose() = 0;
};

}

}