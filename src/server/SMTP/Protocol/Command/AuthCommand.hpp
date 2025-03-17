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
#include <optional>

#include "ICommand.hpp"

#include "AuthentificationMethod.hpp"

namespace SMTP
{

namespace Protocol
{

/**
 * @brief Class that implements AUTH command
 * 
 */
class AuthCommand final : public ICommand
{
public:
    constexpr static std::string_view COMMAND{"AUTH"sv};

    AuthCommand(const AuthentificationMethod auth_method, const std::string_view password);
    ~AuthCommand() = default;

    /**
     * @brief Function that creates response object
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
    std::optional<Response> TryPlain(const Context& options);
    std::optional<Response> TryLogin(const Context& options);

    AuthentificationMethod m_auth_method;
    std::string m_parameter;
};

}

}