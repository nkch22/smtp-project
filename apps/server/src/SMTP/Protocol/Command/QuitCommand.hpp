#pragma once
/**
 * @file QuitCommand.hpp
 * @author Oleksandr
 * @brief 
 * @version 0.1
 * @date 2025-03-04
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
 * @brief Class that implements QUIT command
 * 
 */
class QuitCommand final : public ICommand
{
public:
    constexpr static std::string_view COMMAND{"QUIT"sv};

    QuitCommand() = default;
    ~QuitCommand() = default;

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
};

}

}