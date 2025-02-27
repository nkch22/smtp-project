#pragma once

#include <memory>
#include <string_view>

#include "../Response.hpp"
#include "../Options.hpp"


namespace SMTP
{
    
namespace Protocol
{
using namespace std::string_view_literals;
        
class ICommand
{
public:
    virtual ~ICommand() = default;
    virtual Response CreateResponse(const Options& options) = 0;
};

}

}