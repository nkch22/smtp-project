#pragma once
/**
 * @file AuthCommand.hpp
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
 * @brief Class that implements AUTH command
 * 
 */
class AuthCommand : public ICommand
{
public:
    constexpr static std::string_view COMMAND{"AUTH"sv};

    AuthCommand() = default;
    ~AuthCommand() = default;

    /**
     * @brief Function that creates response object
     * 
     * @param options 
     * @return Response 
     */
    Response CreateResponse(const Options& options) override;

    /**
     * @brief Function that parses smtp-request string
     * 
     * @param request
     * @param options 
     * @return OptionalCommand if command found constructs It else std::nullopt object
     */
    static OptionalCommand TryParseCommand(const std::string& request, const Options& options);
};

}

}