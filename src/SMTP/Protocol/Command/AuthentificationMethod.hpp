#pragma once

#include <format>
#include <cstdint>
#include <sstream>
#include <algorithm>

namespace SMTP
{

namespace Protocol
{

enum class AuthentificationMethod : std::uint8_t
{
    Login,
    Plain,
};

}

}

template<>
struct std::formatter<SMTP::Protocol::AuthentificationMethod, char>
{
    template<class ParseContext>
    constexpr ParseContext::iterator parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template<class FmtContext>
    FmtContext::iterator format(const SMTP::Protocol::AuthentificationMethod method, FmtContext& context) const
    {
        std::ostringstream out{};
        switch(method)
        {
        case SMTP::Protocol::AuthentificationMethod::Plain:
            out << "PLAIN";
            break;
        case SMTP::Protocol::AuthentificationMethod::Login:
            out << "LOGIN";
            break;
        default:
            throw std::runtime_error{"Undefined AuthentificationMethod member!"};
            break;
        }

        return std::ranges::copy(std::move(out).str(), context.out()).out;
    }
};