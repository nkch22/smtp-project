#pragma once
/**
 * @file HeloCommand.hpp
 * @author Oleksandr
 * @brief 
 * @version 0.1
 * @date 2025-03-04
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include <string>

#include "ICommand.hpp"

namespace SMTP
{

namespace Protocol
{

/**
 * @brief Class that implements HELO command
 * 
 */
class HeloCommand : public ICommand
{
public:
    constexpr static std::string_view COMMAND{"HELO"sv};

    /**
     * @brief Construct a new Helo Command object
     * 
     * @param client_domain 
     */
    HeloCommand(const std::string client_domain);
    ~HeloCommand() = default;

    /**
     * @brief Create a Response object
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
private:
    static std::string RemoveWhitespaces(const std::string& string);

    std::string m_client_domain;
}; 

}

}