#pragma once
/**
 * @file EhloCommand.hpp
 * @author Oleksandr
 * @brief 
 * @version 0.1
 * @date 2025-03-04
 * 
 * @copyright Copyright (c) 2025
 * 
 */

#include <vector>

#include "ICommand.hpp"

namespace SMTP
{

namespace Protocol
{

/**
 * @brief Class that implements EHLO Command
 * 
 */
class EhloCommand final : public ICommand
{
public:
    constexpr static std::string_view COMMAND{"EHLO"sv};

    /**
     * @brief Construct a new Ehlo Command object
     * 
     * @param client_domain 
     */
    EhloCommand(const std::string client_domain);
    ~EhloCommand() = default;

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
    static OptionalCommand TryParseCommand(const std::string& request, const Context options);
private:

    /**
     * @brief 
     * 
     * @param options 
     * @return std::vector<std::string> 
     */
    std::vector<std::string> FillExtensions(const Context& options) const;

    /**
     * @brief 
     * 
     * @param string 
     * @return std::string 
     */
    static std::string RemoveWhitespaces(const std::string& string);

    std::string m_client_domain;
};

}

}