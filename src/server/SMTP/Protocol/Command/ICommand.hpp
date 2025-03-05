#pragma once
/**
 * @file ICommand.hpp
 * @author your name (you@domain.com)
 * @brief 
 * @version 0.1
 * @date 2025-03-04
 * 
 * @copyright Copyright (c) 2025
 * 
 */

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
    /**
     * @brief Create a Response object
     * 
     * @param options 
     * @return Response 
     */
    virtual Response CreateResponse(const Options& options) = 0;
};

using OptionalCommand = std::optional<std::unique_ptr<ICommand>>;

}

}