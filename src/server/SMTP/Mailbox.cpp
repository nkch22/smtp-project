#include "Mailbox.hpp"

#include <print>

namespace SMTP
{

Mailbox::Mailbox(const std::string address)
    : m_user{TryParseUser(address)}
    , m_host{TryParseHost(address)}
{
    std::println("{}", address);
    std::println("{}", m_host.value());
}

std::optional<std::string> Mailbox::get_user() const noexcept
{
    return m_user;
}

std::optional<std::string> Mailbox::get_host() const noexcept
{
    return m_host;
}

std::optional<std::string> Mailbox::TryParseUser(const std::string address)
{
    const auto index{address.find("@")};
    if(index != std::string::npos)
    {
        return address.substr(0, index);
    }
    return std::nullopt;
}

std::optional<std::string> Mailbox::TryParseHost(const std::string address)
{
    const auto index{address.find("@")};
    if(index != std::string::npos)
    {
        return address.substr(index + 1, std::size(address) - index - 1);
    }
    return std::nullopt;
}

Mailbox Mailbox::ParseAddressFromRequest(const std::string& request)
{
    const auto start{request.find_first_of("<")};
    const auto end{request.find_last_of(">")};
    const Mailbox mailbox{request.substr(start + 1, end - start - 1)};
    return mailbox;
}

}