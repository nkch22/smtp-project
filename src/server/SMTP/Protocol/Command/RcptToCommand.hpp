#pragma once
/**
 * @file RcptToCommand.hpp
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
 * @brief 
 * 
 */
class RcptToCommand : public ICommand
{
public:
    constexpr static std::string_view COMMAND{"RCPT"sv};

    RcptToCommand() = default;
    ~RcptToCommand() = default;

    /**
     * @brief Create a Response object
     * 
     * @param options 
     * @return Response 
     */
    Response CreateResponse(const Options& options) override;

    /**
     * @brief 
     * 
     * @param request 
     * @param options 
     * @return OptionalCommand 
     */
    static OptionalCommand TryParseCommand(const std::string& request, const Options& options);
};

}

}