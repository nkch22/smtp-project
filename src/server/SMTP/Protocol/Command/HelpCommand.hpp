#pragma once
/**
 * @file HelpCommand.hpp
 * @author Oleksandr
 * @brief 
 * @version 0.1
 * @date 2025-03-05
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include "ICommand.hpp"

namespace SMTP
{

namespace Protocol
{

/**
 * @brief Class that implements HELP command
 * 
 */
class HelpCommand final : public ICommand
{
public:
    constexpr static std::string_view COMMAND{"HELP"};

    /**
     * @brief Construct a new Help Command object
     * 
     */
    HelpCommand() = default;
    ~HelpCommand() = default;

    /**
     * @brief Create a Response object
     * 
     * @param options 
     * @return Response 
     */
    Response CreateResponse(Context& options) override;

    /**
     * @brief Function that parses smtp-request string
     * 
     * @param request 
     * @param options 
     * @return OptionalCommand if command found constructs It else std::nullopt object 
     */
    static OptionalCommand TryParseCommand(const std::string& request, const Context& options);

private:
    static std::string FormatData(const std::string_view data);
};
    
}

}