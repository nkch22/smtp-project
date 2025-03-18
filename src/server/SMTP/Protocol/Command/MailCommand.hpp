#pragma once
/**
 * @file MailFromCommand.hpp
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
 * @brief Class that implements MAIL command
 * 
 */
class MailCommand final : public ICommand
{
public:
    constexpr static std::string_view COMMAND{"MAIL"sv};
    constexpr static std::string_view END_OF_MAIL{"\r\n.\r\n"sv};

    MailCommand(const Mailbox& mailbox);
    ~MailCommand() = default;

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