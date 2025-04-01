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
#include "../../Mailbox.hpp"

namespace SMTP
{

namespace Protocol
{

/**
 * @brief 
 * 
 */
class RcptCommand final : public ICommand
{
public:
    constexpr static std::string_view COMMAND{"RCPT"sv};

    RcptCommand(const Mailbox& mailbox);
    ~RcptCommand() = default;

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
    Mailbox m_mailbox;
};

}

}