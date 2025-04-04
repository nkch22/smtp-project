#pragma once
/**
 * @file Parser.hpp
 * @author Oleksandr
 * @brief 
 * @version 0.1
 * @date 2025-03-04
 * 
 * @copyright Copyright (c) 2025
 * 
 */

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

/**
 * @brief Class that parses SMTP requests
 * 
 */
class Parser
{
public:
    Parser() = default;
    ~Parser() = default;
    /**
     * @brief 
     * 
     * @param request 
     * @param custom_options 
     * @return OptionalCommand 
     */
    OptionalCommand TryParseRequest(const std::string& request, const Context& custom_options) const;
private:

    /**
     * @brief 
     * 
     * @param string 
     * @return std::string 
     */
    static std::string ToUpper(const std::string& string);
};

}

}