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
#include "../Options.hpp"

namespace SMTP
{

namespace Protocol
{

/**
 * @brief Class that implements EHLO Command
 * 
 */
class EhloCommand : public ICommand
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
    Response CreateResponse(const Options& options) override;

    /**
     * @brief 
     * 
     * @param request 
     * @param options 
     * @return OptionalCommand if command found constructs It else std::nullopt object 
     */
    static OptionalCommand TryParseCommand(const std::string& request, const Options options);
private:

    /**
     * @brief 
     * 
     * @param options 
     * @return std::vector<std::string> 
     */
    std::vector<std::string> FillExtensions(const Options& options) const;

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