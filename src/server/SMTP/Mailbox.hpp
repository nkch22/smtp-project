#pragma once

#include <string>
#include <optional>

namespace SMTP
{

class Mailbox 
{
public:
    Mailbox() = default;
    Mailbox(const Mailbox& other) = default;
    Mailbox& operator=(const Mailbox& other) = default;
    Mailbox(Mailbox&& other) = default;
    Mailbox& operator=(Mailbox&& other) = default;
    Mailbox(const std::string address);

    std::optional<std::string> get_user() const noexcept;
    std::optional<std::string> get_host() const noexcept;

    static Mailbox ParseAddressFromRequest(const std::string& request);
private:
    static std::optional<std::string> TryParseUser(const std::string address);
    static std::optional<std::string> TryParseHost(const std::string address);

    std::optional<std::string> m_user;
    std::optional<std::string> m_host;
};

}