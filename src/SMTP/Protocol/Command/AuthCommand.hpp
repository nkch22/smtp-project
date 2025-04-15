#pragma once

#include <optional>

#include "ICommand.hpp"

#include "AuthentificationMethod.hpp"

namespace SMTP
{

namespace Protocol
{

class AuthCommand final : public ICommand
{
public:
    constexpr static std::string_view COMMAND{"AUTH"sv};

    AuthCommand(
        const AuthentificationMethod auth_method, 
        const std::optional<std::string> encoded_username_password
    );
    AuthCommand(
        const AuthentificationMethod auth_method, 
        const std::optional<std::string> encoded_username, 
        const std::optional<std::string> encoded_password
    );
    ~AuthCommand() = default;

    Response Execute(Context& options) override;
    static OptionalCommand TryParseCommand(const std::string& request, const Context& options);
private:
    Response DoPlain(Context& options);
    Response DoLogin(Context& options);

    static OptionalCommand TryParseCommandStart(
        const std::size_t found_auth,
        const std::string& request, 
        const Context& options
    );

    bool CreateUserIfNot(Context& options);

    static OptionalCommand TryParseCommandContinuation(
        const std::string& request, 
        const Context& options
    );

    static std::optional<std::string> TryReceiveUsername(
        const AuthentificationMethod auth_method, 
        const std::optional<std::string> encoded_username
    );

    static std::optional<std::string> TryReceivePassword(
        const AuthentificationMethod auth_method, 
        const std::optional<std::string> encoded_password
    );

    AuthentificationMethod m_auth_method;
    std::optional<std::string> m_username;
    std::optional<std::string> m_password;
};

}

}