#pragma once

#include <cstdint>
#include <string>
#include <optional>

namespace SMTP
{

struct ServerOptions
{
    std::optional<std::size_t> maximum_byte_amount;
    std::string domain_name;
};

}